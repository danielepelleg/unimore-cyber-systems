#include <cstdint>
#include <iostream>

#include "pbm.h"

int main(int argc, char * argv[]) {
	if (argc != 2)
		return EXIT_FAILURE;
	std::string input_file = argv[1];

	BinaryImage bimg;
	bimg.ReadFromPBM(input_file);
	std::cout << "Binary IMG Size: " << bimg.ImageData.size();

	Image img = BinaryImageToImage(bimg);
	std::cout << "Image IMG Size: " << img.ImageData.size();

	return EXIT_SUCCESS;
}