#include <cstdint>
#include <cstdlib>
#include <fstream>

#include "lzs.h"

int main(int argc, char * argv[]) {
	if (argc != 3) {
		return EXIT_FAILURE;
	}
	std::string input_file = argv[1];
	std::string output_file = argv[2];

	std::ifstream is(input_file, std::ios::binary);
	std::ofstream os(output_file, std::ios::binary);

	lzs_decompress(is, os);

	return EXIT_SUCCESS;
}