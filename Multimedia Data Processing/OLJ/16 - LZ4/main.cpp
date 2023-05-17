#include <string>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <vector>

template<typename T>
std::istream& raw_read(std::istream &is, T &val, size_t size = sizeof(T)) {
	return is.read(reinterpret_cast<char *>(&val), size);
}

bool readHeader(std::istream &is) {
	uint32_t magic_number;
	is >> magic_number;
	if (magic_number != 0x184C2103) {
		return false;
	}

	uint32_t length_uncompressed;
	is >> length_uncompressed;
	std::cout << "Uncompressed File Length: " << length_uncompressed << std::endl;
	uint32_t constant_value;
	is >> constant_value;
	if (constant_value != 0x4D000000) {
		return false;
	}
}

bool decompress(std::string input_file, std::string output_file) {
	std::ifstream is(input_file, std::ios::binary);
	readHeader(is);

	// Compressed Blocks
	uint8_t token;
	uint32_t BlockLength;
	// Read all BLOCKS
	while(is.read(reinterpret_cast<char *>(&BlockLength), 4)){
		raw_read(is, token);
		size_t literalsLength = token >> 4;
		if (literalsLength == 15) {
			// Read More
			uint8_t additionalByte;
			while (raw_read(is, additionalByte)) {
				literalsLength += additionalByte;
				if (additionalByte != 255) break;
			}
		}
		// Read Literals
		std::vector<uint8_t> literals;
		for (size_t i = 0; i < literalsLength; i++) {
			uint8_t literal;
			raw_read(is, literal);
			literals.push_back(literal);
		}
	}
}

int main(int argc, char * argv[]) {
	if (argc != 3) {
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}