#pragma once
#include <fstream>

class bitwriter {
private:
	std::ostream &os_;
	uint8_t buffer_;
	size_t nbits_;

	std::ostream& write_bit(uint32_t u);

public:
	bitwriter(std::ostream &os);
	~bitwriter();

	std::ostream& write(uint32_t u, size_t n);
	std::ostream& operator()(uint32_t u, size_t n);
	void flush(uint32_t u);
};

class bitreader {
private:
	std::istream &is_;
	uint8_t buffer_;
	size_t nbits_;

	uint32_t read_bit();
public:
	bitreader(std::istream &is);
	~bitreader();

	std::istream& read(uint32_t &u, size_t n);
	std::istream& read(int32_t &i, size_t n);
};

int count_bits(int k);

