#pragma once

#include <fstream>

template<typename T>
std::istream& raw_read(std::istream &is, T &val, size_t size = sizeof(T)) {
	return is.read(reinterpret_cast<char *>(&val), size);
}

template<typename T>
std::ostream& raw_write(std::ostream &os, const T &val, size_t size = sizeof(T)) {
	return os.write(reinterpret_cast<const char *>(&val), size);
}

class bitreader {
	std::istream& is_;
	uint8_t buffer_;
	size_t nbits_;

	uint32_t read_bit() {
		if (nbits_ == 0) {
			raw_read(is_, buffer_);
			nbits_ = 8;
		}
		--nbits_;
		return (buffer_ >> nbits_) & 1;
	}
public:
	bitreader(std::istream &is) : is_(is), nbits_(0) {}

	std::istream& read(uint32_t &u, size_t n) {
		u = 0;
		while (n-- > 0) {
			u = (u << 1) | read_bit();
		}
		return is_;
	}
};

uint32_t read_length(bitreader &br);

void lzs_decompress(std::istream &is, std::ostream &os);