#include "bit.h"
#include "raw.h"

bitwriter::bitwriter(std::ostream &os) : os_(os), nbits_(0) {};

std::ostream& bitwriter::write_bit(uint32_t u) {
	buffer_ = (buffer_ << 1) | (u & 1);
	if (++nbits_ == 8) {
		raw_write(os_, buffer_);
		nbits_ = 0;
	}
	return os_;
}

std::ostream& bitwriter::write(uint32_t u, size_t n) {
	for (size_t i = 0; i < n; ++i) {
		write_bit(u >> (n - 1 - i));
	}
	return os_;
}

std::ostream& bitwriter::operator()(uint32_t u, size_t n) {
	return write(u, n);
}

void bitwriter::flush(uint32_t u = 0) {
	while (nbits_ > 0) {
		write_bit(u);
	}
}

bitwriter::~bitwriter() {
	flush();
}

int count_bits(int k) {
	int count = 0;
	while (k > 0) {
		++count;
		k /= 2;
	}
	return count;
}

bitreader::bitreader(std::istream &is) : is_(is), nbits_(0) {}

uint32_t bitreader::read_bit() {
	if (nbits_ == 0) {
		raw_read(is_, buffer_);
		nbits_ = 8;
	}
	--nbits_;
	return (buffer_ >> nbits_) & 1;
}

std::istream& bitreader::read(uint32_t &u, size_t n) {
	u = 0;
	while (n-- > 0) {
		u = (u << 1) | read_bit();
	}
	return is_;
}

std::istream& bitreader::read(int32_t &i, size_t n) {
	uint32_t u;
	read(u, n);
	i = static_cast<int32_t>(u);
	return is_;
}

bitreader::~bitreader() {}