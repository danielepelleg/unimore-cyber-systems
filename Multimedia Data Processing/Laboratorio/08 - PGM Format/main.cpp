#include <cstdlib>
#include <iostream>
#include <fstream>
#include "pgm.h"

void error(const std::string &msg) {
	std::cout << msg;
	exit(EXIT_FAILURE);
}

auto make_test_pattern() {
	matrix<uint8_t> img(256, 256);
	for (int r = 0; r < img.rows(); ++r) {
		for (int c = 0; c < img.cols(); ++c) {
			img(r, c) = r;
		}
	}
	return img;
}

auto flip(matrix<uint8_t> &img) {
	// flip the first half rows of the image with the bottom half
	for (int r = 0; r < img.rows()/2; ++r) {
		for (int c = 0; c < img.cols(); ++c) {
			std::swap(img(r, c), img(img.rows() - 1 - r, c));
		}
	}
}

int main(int argc, char *argv[]) {
	
	matrix<uint8_t> img = make_test_pattern();

	write("test_plain.pgm", img, pgm_mode::plain);
	write("test_bin.pgm", img, pgm_mode::binary);
	
	img = read("frog_asc.pgm");
	if (img.empty()) {
		error("Cannot read the image");
	}
	flip(img);
	write("flipped_frog.pgm", img, pgm_mode::binary);

	_CrtDumpMemoryLeaks();
	return EXIT_SUCCESS;
} 