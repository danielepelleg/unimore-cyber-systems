#include <string>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <vector>

template<typename T>
std::istream& raw_read(std::istream &is, T &val, size_t size = sizeof(T)) {
	return is.read(reinterpret_cast<char *>(&val), size);
}

std::vector<uint8_t>& concatenate_vector(std::vector<uint8_t>& a, std::vector<uint8_t>& b) {
	for (uint8_t x : b) a.push_back(x);
	return a;
}

std::string& addLiterals(std::string &sequence, std::vector<uint8_t> &literals) {
	for (auto l : literals) {
		sequence += (char)l;
	}
	return sequence;
}

bool readHeader(std::istream &is) {
	uint32_t magic_number;
	raw_read(is, magic_number);
	if (magic_number != 0x184C2103) {
		return false;
	}

	uint32_t length_uncompressed;
	raw_read(is, length_uncompressed);
	std::cout << "Uncompressed File Length: " << length_uncompressed << std::endl;
	uint32_t constant_value;
	raw_read(is, constant_value);
	if (constant_value != 0x4D000000) {
		return false;
	}
	return true;
}

bool lz4_decompress(std::string input_file, std::string output_file) {
	std::ifstream is(input_file, std::ios::binary);
	readHeader(is);
	std::ofstream os(output_file, std::ios::binary);
	if(!os) {
		return false;
	}
	// Compressed Blocks
	uint8_t token;
	uint32_t BlockLength;
	std::string(decoded);
	std::vector<uint8_t> dictionary;
	// Read all BLOCKS
	while(is.read(reinterpret_cast<char *>(&BlockLength), 4)){
		uint32_t readBytes = 0;
		// Read a SEQUENCE (Literals, Match Copy)
		while(raw_read(is, token)){
			++readBytes;
			size_t literalsLength = token >> 4;
			if (literalsLength == 15) {
				// Read More
				uint8_t additionalByte;
				while (raw_read(is, additionalByte)) {
					++readBytes;
					literalsLength += additionalByte;
					if (additionalByte != 255) break;
				}
			}
			// Read Literals
			std::vector<uint8_t> literals;
			for (size_t i = 0; i < literalsLength; i++) {
				uint8_t literal;
				raw_read(is, literal);
				++readBytes;
				literals.push_back(literal);
			}
			concatenate_vector(dictionary, literals);
			addLiterals(decoded, literals);
			os << decoded;
			decoded.clear();
			// Check if QUIT
			// (must end reading literals)
			if (readBytes == BlockLength) {
				break;
			}
			// 
			uint16_t offset;
			raw_read(is, offset);
			readBytes += 2;
			size_t match_length = (token & 0b1111);
			if (match_length == 15) {
				// Read More
				uint8_t additionalByte;
				while (raw_read(is, additionalByte)) {
					++readBytes;
					match_length += additionalByte;
					if (match_length != 255) break;
				}
			}
			match_length += 4;
			for (size_t i = 0; i < match_length; ++i) {
				char c = (char)(*(dictionary.end() - offset));
				decoded += c;
				dictionary.push_back(c);
			}
			os << decoded;
			decoded.clear();
		}
		
	}
	return true;
}

int main(int argc, char * argv[]) {
	if (argc != 3) {
		return EXIT_FAILURE;
	}
	std::string input_filename = argv[1];
	std::string output_filename = argv[2];
	
	lz4_decompress(input_filename, output_filename);
	return EXIT_SUCCESS;
}