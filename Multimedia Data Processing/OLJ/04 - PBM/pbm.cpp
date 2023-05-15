#include <fstream>
#include <iostream>
#include <cmath>

#include "pbm.h"

template<typename T>
std::istream& raw_read(std::istream &is, T &val, size_t size = sizeof(T)) {
	return is.read(reinterpret_cast<char *>(&val), size);
}

bool BinaryImage::ReadFromPBM(const std::string &filename) {
	std::ifstream is(filename, std::ios::binary);
	if (!is)
		return false;
	std::string magic_number;
	std::getline(is, magic_number);

	// Skip Comments
	while (is.peek() == '#') {
		std::string dump;
		std::getline(is, dump);
	}

	is >> W >> H;
	std::cout << "Width: " << W << "\nHeight: " << H << std::endl;
	//resize(H, W);

	char c;
	if (!is.get(c) || c != '\n')
		return false;

	ReadData(is);

	return true;
}

void BinaryImage::ReadData(std::istream &is) {
	uint8_t missing_bits = W % 8;
	for(int r = 0; r < H; ++r) {
		for (int c = 0; c < W / 8; ++c) {
			uint8_t pixel;
			raw_read(is, pixel);
			ImageData.push_back((uint8_t)pixel);
		}
		if (missing_bits != 0) {
			uint8_t pixel;
			raw_read(is, pixel);
			ImageData.push_back((uint8_t)pixel);
		}
	}
}

uint8_t convert_BW(uint8_t pixel) {
	if (pixel == 1)
		return 0;		// black
	else return 255;	// white
}

Image BinaryImageToImage(const BinaryImage &bimg) {
	Image img;
	img.H = bimg.H;
	img.W = bimg.W;

	uint8_t missing_bits = img.W % 8;
	size_t index = 0;
	for (int r = 0; r < img.H; ++r) {
		for (int c = 0; c < img.W / 8; ++c) {
			img.ImageData.push_back(convert_BW((bimg[index] & 0b10000000) >> 7));
			img.ImageData.push_back(convert_BW((bimg[index] & 0b01000000) >> 6));
			img.ImageData.push_back(convert_BW((bimg[index] & 0b00100000) >> 5));
			img.ImageData.push_back(convert_BW((bimg[index] & 0b00010000) >> 4));
			img.ImageData.push_back(convert_BW((bimg[index] & 0b00001000) >> 3));
			img.ImageData.push_back(convert_BW((bimg[index] & 0b00000100) >> 2));
			img.ImageData.push_back(convert_BW((bimg[index] & 0b00000010) >> 1));
			img.ImageData.push_back(convert_BW((bimg[index] & 0b00000001) >> 0));
			index++;
		}
		if (missing_bits != 0) {
			for (int i = 0; i < missing_bits; ++i) {
				img.ImageData.push_back(convert_BW((bimg[index] & (uint8_t) pow(2, 7 - i)) >> (7 - i)));
			}
			index++;
		}
	}

	return img;
}