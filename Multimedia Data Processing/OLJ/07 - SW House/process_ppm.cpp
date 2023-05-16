#include <string>
#include <array>
#include <fstream>
#include <iostream>

#include "mat.h"
#include "ppm.h"

uint16_t read2BytesBigEndian(std::istream &is) {
	uint8_t first;
	uint8_t second;
	is.read(reinterpret_cast<char *>(&first), 1);
	is.read(reinterpret_cast<char *>(&second), 1);
	uint16_t result = (first << 8) | second;
	return result;
}

bool LoadPPM(const std::string &filename, mat<vec3b> &img) {
	std::ifstream is(filename, std::ios::binary);
	if (!is) {
		return false;
	}

	std::string magic_number;
	std::getline(is, magic_number);
	if (magic_number != "P6") {
		return false;
	}

	// Handle Comments
	while (is.peek() == '#') {
		is.get();
		std::string comment;
		std::getline(is, comment);
	}

	int width, height, maxVal;
	is >> width >> height >> maxVal;
	std::cout << "Width: " << width << "\nHeight: " << height << std::endl;
	if (maxVal < 0 || maxVal > 65536) {
		return false;
	}
	std::cout << "Max Value: " << maxVal << std::endl;

	// NewLine
	is.get();

	if (maxVal < 256) {
		img.resize(height, width);
		// RGB - 1 Byte
		for (int r = 0; r < img.rows(); ++r) {
			for (int c = 0; c < img.cols(); ++c) {
				vec3b rgb;
				is.read(reinterpret_cast<char *>(&rgb[0]), 1);
				is.read(reinterpret_cast<char *>(&rgb[1]), 1);
				is.read(reinterpret_cast<char *>(&rgb[2]), 1);
				img(r, c) = rgb;
			}
		}

	}
	else {
		// RGB - 2 Byte
		img.resize(height, 2*width);
		for (int r = 0; r < img.rows(); ++r) {
			for (int c = 0; c < img.cols(); ++c) {
				vec3b rgb;
				rgb[0] = read2BytesBigEndian(is);
				rgb[1] = read2BytesBigEndian(is);
				rgb[2] = read2BytesBigEndian(is);
				img(r, c) = rgb;
			}
		}
	}

	return true;
}

void SplitRGB(const mat<vec3b> &img, mat<uint8_t> &img_r, mat<uint8_t> &img_g, mat<uint8_t> &img_b) {
	img_r.resize(img.rows(), img.cols());
	img_g.resize(img.rows(), img.cols());
	img_b.resize(img.rows(), img.cols());

	for (int r = 0; r < img.rows(); ++r) {
		for (int c = 0; c < img.cols(); ++c) {
			auto frame_rgb = img(r, c);
			img_r(r, c) = frame_rgb[0];
			img_g(r, c) = frame_rgb[1];
			img_b(r, c) = frame_rgb[2];
		}
	}
}

/*
int main(int argc, char * argv[]) {
	if (argc != 2) {
		return EXIT_FAILURE;
	}
	std::string input_file = argv[1];

	mat<vec3b> img_rgb;
	LoadPPM(input_file,img_rgb);

	mat<uint8_t> img_r;
	mat<uint8_t> img_g;
	mat<uint8_t> img_b;

	return EXIT_SUCCESS;
}
*/