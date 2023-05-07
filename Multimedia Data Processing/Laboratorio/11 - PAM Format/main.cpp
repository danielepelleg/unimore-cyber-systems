#include<cstdlib>
#include<cstdio>
#include<iostream>
#include<fstream>
#include<string>

#include "matrix.h"
#include "pam.h"

auto make_test_pattern() {
	Matrix<uint8_t> img(256, 256);
	for (int r = 0; r < img.rows(); ++r) {
		for (int c = 0; c < img.cols(); ++c) {
			img(r, c) = r;
		}
	}
	return img;
}

int error(std::string msg) {
	std::cout << msg << std::endl;
	return EXIT_FAILURE;
}

int main(int argc, char * argv[]) {
	// Write Test IMG
	Matrix<uint8_t> img = make_test_pattern();
	write_pam("test_grayscale.pam", img);

	// Read Frog Image (Good, 1 Channel Grayscale)
	Matrix<uint8_t> frog_img;
	read_pam("frog.pam", frog_img);

	// Read Laptop Image (Error, 3 Channels RGB)
	Matrix<uint8_t> laptop_image;
	read_pam("laptop.pam", laptop_image);

	return EXIT_SUCCESS;
}