#include <iostream>
#include <cstdlib>
#include <string>

#include "base64.h"



int main(int argc, char* argv[]) {
	if (argc != 2) {
		return EXIT_FAILURE;
	}
	std::string filename = argv[1];
	std::string output = base64_decode(filename);

	return EXIT_SUCCESS;
}