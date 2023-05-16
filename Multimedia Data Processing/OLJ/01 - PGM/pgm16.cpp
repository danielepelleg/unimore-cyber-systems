#include <fstream>
#include <iostream>

#include "pgm16.h"

bool load(const std::string& filename, mat<uint16_t> &img, uint16_t& maxvalue) {
	std::ifstream is(filename, std::ios::binary);
	if (!is)
		return false;
	std::string magic_number;
	std::getline(is, magic_number);
	std::cout << magic_number << std::endl;

	// Handle Comments
	if (is.peek() == '#') {
		std::string dump;
		std::getline(is, dump);
	}

	uint32_t width, heigth;
	is >> width;
	is >> heigth;
	is >> maxvalue;
	std::cout << "Width: " << width << "\nHeight: " << heigth << "\nMax Value: " << maxvalue << std::endl;
	img.resize(heigth, width);

	char c;
	if (!is.get(c) || c != '\n')
		return false;


	if (maxvalue >= 256) {
		for (int r = 0; r < img.rows(); ++r) {
			for (int c = 0; c < img.cols(); ++c) {
				uint8_t first;
				uint8_t second;
				is.read(reinterpret_cast<char *>(&first), sizeof(first));
				is.read(reinterpret_cast<char *>(&second), sizeof(second));
				uint16_t value = (first << 8) | second;
				img(r, c) = value;
			}
		}
	}
	else {
		for (int r = 0; r < img.rows(); ++r) {
			for (int c = 0; c < img.cols(); ++c) {
				uint8_t value;
				is.read(reinterpret_cast<char *>(&value), sizeof(value));
				img(r, c) = static_cast<uint16_t>(value);
			}
		}
	}
	return true;
}