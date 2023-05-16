#pragma once

#include <string>
#include <vector>
#include <fstream>

struct BinaryImage {
	int W;
	int H;
	std::vector<uint8_t> ImageData;

	uint8_t& operator[](int i) { return ImageData[i]; }
	const uint8_t& operator[](int i) const { return ImageData[i]; }
	 
	bool ReadFromPBM(const std::string &filename);
	void ReadData(std::istream &is);
};

struct Image {
	int W;
	int H;
	std::vector<uint8_t> ImageData;
};

Image BinaryImageToImage(const BinaryImage& bimg);