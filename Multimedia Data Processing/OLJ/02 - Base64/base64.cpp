#include "base64.h"

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

struct bitwriter {
	uint8_t buffer_;
	std::ostream& os_;
	size_t nbits_;

	bitwriter(std::ostream &os) : os_(os), nbits_(0) {}

	~bitwriter() {
		flush();
	}

	std::ostream& write_bit(uint8_t u){
		buffer_ = (buffer_ << 1) | (u & 1);
		if (++nbits_ == 8) {
			os_.write(reinterpret_cast<char *>(&buffer_), sizeof(buffer_));
			nbits_ = 0;
		}
		return os_;
	}

	std::ostream& write(uint8_t u, size_t n) {
		for (size_t i = 0; i < n; ++i)
			write_bit(u >> (n - 1 - i));
		return os_;
	}

	void flush(uint8_t u = 0) {
		while (nbits_ > 0)
			write_bit(u);
	}
};

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

char decodeChar(char c) {
	// Base64 Array Symbols
	const std::string base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

	// Decoding
	if (c == '=') {
		return static_cast<char>(0);
	}

	std::size_t pos = base64_chars.find(c);
	if (pos == std::string::npos) {
		std::cerr << "Carattere non valido in input: " << (int)c << std::endl;
		return static_cast<char>(0);
	}

	return static_cast<char>(pos);
}

char encodeChar(uint8_t val) {
	const std::string base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

	if (val < base64_chars.size()) {
		return base64_chars[val];
	}
	return '#'; // error
}

std::string base64_decode(const std::string& input) {
	std::string encoded;
	if (input.empty())
		return encoded;

	for (const auto &c : input)
		encoded.push_back(decodeChar(c));

	std::string decoded;
	std::ostringstream os(decoded);
	bitwriter bw(os);
	for (size_t i = 0; i < encoded.length(); ++i) {
		// Write using 6 bits
		bw.write(encoded[i], 6);
	}
	decoded = os.str();
	return decoded;
}

std::string base64_encode(const std::string& input) {
	std::stringstream stream(input);

	uint8_t padding = 128;
	size_t text_size = input.size();
	while (text_size % 3 != 0) {
		stream.write(reinterpret_cast<char *>(&padding), 1);
		++text_size;
	}

	std::string output;
	bitreader br(stream);
	uint8_t value;
	while (br.read(value, 6)) {
		output.push_back(encodeChar(value));
	}
	return output;
}
