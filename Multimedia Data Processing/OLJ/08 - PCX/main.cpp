#include <iostream>
#include <string>

#include "types.h"
#include "pcx.h"

int main(int argc, char * argv[]) {
	if (argc != 2) {
		return EXIT_FAILURE;
	}
	std::string filename = argv[1];
	/* 1BPP - B/W
	mat<uint8_t> img1bpp;
	load_pcx(filename, img1bpp);
	*/

	/* 24BPP - RGB */
	mat<vec3b> img24bpp;
	load_pcx(filename, img24bpp);
	return EXIT_SUCCESS;
}