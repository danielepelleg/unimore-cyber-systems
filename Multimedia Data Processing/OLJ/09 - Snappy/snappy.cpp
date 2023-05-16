#include <fstream>
#include <iostream>
#include <vector>
#include <cstdint>
#include <iterator>

uint32_t read_preamble(std::istream& is);
bool read_element(std::istream& is, std::ostream& os, std::vector<uint8_t>& dictionary);
bool read_literal(std::istream& is, uint8_t literal_length, std::vector<uint8_t>& dictionary);
bool read_copy_1(std::istream& is, uint8_t tag, std::vector<uint8_t>& dictionary);
bool read_copy_2(std::istream& is, uint8_t tag, std::vector<uint8_t>& dictionary);
bool read_copy_4(std::istream& is, uint8_t tag, std::vector<uint8_t>& dictionary);

uint32_t read_preamble(std::istream& is) {
	uint8_t varint_bit;
	std::vector<uint8_t> bytes;

	do {
		uint8_t byte;
		is.read(reinterpret_cast<char*>(&byte), 1);
		// 0x80 -> 1000.0000
		// varint it's the msb
		varint_bit = (byte & 0x80) >> 7;
		// 0x7F -> 0111.1111
		bytes.push_back(byte & 0x7F);
	} while (varint_bit == 1);
	return 0;
}

bool read_element(std::istream& is, std::ostream& os, std::vector<uint8_t>& dictionary) {
	uint8_t tag = 0;
	is.read(reinterpret_cast<char*>(&tag), 1);
	uint8_t length = tag >> 2;
	// 0x03 -> 0000.0011
	// type is the 2 lsb of tag
	uint8_t type = tag & 0x03;
	if (type == 0) {
		read_literal(is, length, dictionary);
	}
	else if (type == 1) {
		read_copy_1(is, tag, dictionary);
	}
	else if (type == 2) {
		read_copy_2(is, tag, dictionary);
	}
	else if (type == 3) {
		read_copy_4(is, tag, dictionary);
	}
	else {
		return false;
	}
	return true;
}

bool read_literal(std::istream& is, uint8_t literal_length, std::vector<uint8_t>& dictionary) {
	uint32_t length = 0;
	if (literal_length < 60) {
		length = literal_length + 1;
	}
	else {
		is.read(reinterpret_cast<char*>(&length), literal_length - 59);
		length += 1;
	}

	std::vector<char> data(length);
	is.read(data.data(), data.size());
	for (char const &x : data){
		dictionary.push_back((uint8_t)x);
	}
	return true;
}

bool read_copy_1(std::istream& is, uint8_t tag, std::vector<uint8_t>& dictionary) {
	uint8_t length = 0;
	uint16_t offset = 0;
	// 7 -> 0000.0111
	// takes bits 4,3,2 to obtain length - 4
	// >> 2 - removes bit 0,1
	length = (tag >> 2) & 7;
	length += 4;
	// 0xE0 -> 1110.0000
	// takes bits 7,6,5
	offset = (tag & (0xE0));
	// transform to 11 bit -> 111.0000.0000
	offset = offset << 3;

	uint8_t next_byte;
	// read 1 byte
	is.read(reinterpret_cast<char*>(&next_byte), 1);
	// add 8 lsb of byte to the offset
	offset |= next_byte;

	for (int i = 0; i < length; i++) {
		uint8_t d = *(dictionary.end() - offset);
		dictionary.push_back(d);
	}
	return true;
}

bool read_copy_2(std::istream& is, uint8_t tag, std::vector<uint8_t>& dictionary) {
	uint8_t length = 0;
	uint16_t offset = 0;
	// takes the 6 msb to obtain length - 1
	length = tag >> 2;
	length += 1;

	is.read(reinterpret_cast<char*>(&offset), 2);

	for (int i = 0; i < length; i++) {
		uint8_t d = *(dictionary.end() - offset);
		dictionary.push_back(d);
	}
	return true;
}

bool read_copy_4(std::istream& is, uint8_t tag, std::vector<uint8_t>& dictionary) {
	uint8_t length = 0;
	uint32_t offset = 0;
	// takes the 6 msb to obtain length - 1
	length = tag >> 2;
	length += 1;

	is.read(reinterpret_cast<char*>(&offset), 4);

	for (int i = 0; i < length; i++) {
		uint8_t d = *(dictionary.end() - offset);
		dictionary.push_back(d);
	}
	return true;
}

int main(int argc, char* argv[]) {
	if (argc != 3) return 1;
	std::ifstream is(argv[1], std::ios::binary);
	std::ofstream os(argv[2], std::ios::binary);

	std::vector<uint8_t> dictionary;

	read_preamble(is);
	while (is) {
		int b = is.peek();
		if (b == -1) 
			break;
		read_element(is, os, dictionary);
	}
	os.write(reinterpret_cast<char*>(dictionary.data()), dictionary.size());
}
