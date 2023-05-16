#include <vector>
#include <iostream>
#include <string>
#include <sstream>

#include "mat.h"

struct Packbits {
	uint8_t EOD = 128;
	int counter_;
	std::vector<uint8_t> buffer_;
	
	Packbits() : counter_(1) {}

	void operator++() { ++counter_; }

	void operator--() { --counter_; }

	void reset_counter() { counter_ = 0; }

	uint8_t run() { return 257 - counter_; }

	uint8_t copy() { return counter_ - 1;  }

	void add(uint8_t element) { buffer_.push_back(element); }

	void clear() { buffer_.clear(); }

	auto back() { return buffer_.back(); }
};

void PackBitsEncode(const mat<uint8_t>& img, std::vector<uint8_t>& encoded) {
	uint8_t previous = img(0,0);
	Packbits PB;
	PB.add(previous);

	for (int r = 0; r < img.rows(); ++r) {
		for (int c = 0; c < img.cols(); ++c) {
			// Skip First Frame
			if (r == 0 && c == 0) {
				continue;
			}
			// If = to last increment Counter
			if (img(r, c) == previous) {
				// Write a COPY
				if (!PB.buffer_.empty()) {
					PB.buffer_.pop_back();
					--PB.counter_;
					encoded.push_back(PB.copy());
					for (const auto &x : PB.buffer_) {
						encoded.push_back(x);
					}
					PB.clear();
					PB.reset_counter();
				}
				++PB.counter_;
			}
			else {
				// Write a RUN
				if (PB.counter_ > 1 && PB.buffer_.empty()) {
					encoded.push_back(PB.run());
					encoded.push_back(previous);
					PB.reset_counter();
				}
				PB.add(img(r,c));
				++PB.counter_;
			}
			previous = img(r, c);
		}
	}
	encoded.push_back(PB.EOD);
}

struct bitreader {
	uint8_t buffer_ = 0;
	size_t nbits_ = 0;
	std::istream& is_;

	bitreader(std::istream &is) : is_(is) {}

	uint8_t read_bit() {
		if (nbits_ == 0) {
			is_.read(reinterpret_cast<char *>(&buffer_), 1);
			nbits_ = 8;
		}
		--nbits_;
		return (buffer_ >> nbits_) & 1;
	}

	std::istream& read(uint8_t &u, size_t n) {
		u = 0;
		while (n-- > 0) {
			u = (u << 1) | read_bit();
		}
		return is_;
	}
};

char Base64Convert(uint8_t val) {
	const std::string base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

	if (val < base64_chars.size()) {
		return base64_chars[val];
	}
	return '#'; // error
}

std::string Base64Encode(const std::vector<uint8_t> &v) {
	std::string text;
	std::stringstream stream(text);
	
	for (auto &el : v) {
		uint8_t item = el;
		stream.write(reinterpret_cast<char *>(&item), 1);
	}

	uint8_t padding = 128;
	size_t text_size = v.size();
	while (text_size % 3 != 0) {
		stream.write(reinterpret_cast<char *>(&padding), 1);
		++text_size;
	}
	
	std::string output;
	bitreader br(stream);
	uint8_t value;
	while (br.read(value, 6)) {
		output.push_back(Base64Convert(value));
	}
	return output;
}

/*
int main() {
	std::vector<uint8_t> v = { 0xdd, 0xff, 0x80 };
	Base64Encode(v);
}
*/