#include <cstdint>
#include <cstdlib>

#include "pgm16.h"

int main(int argc, char * argv[]) {
	if (argc != 2) {
		return EXIT_FAILURE;
	}
	std::string filename = argv[1];
	mat<uint16_t> img;
	uint16_t max_value;
	load(filename, img, max_value);
	return EXIT_SUCCESS;
}