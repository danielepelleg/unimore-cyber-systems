#include <vector>
#include <string>

#include "lzs.h"

uint32_t read_length(bitreader &br) {
	uint32_t first_two;
	br.read(first_two, 2);
	if (first_two == 0b00)
		return 2;
	if (first_two == 0b01)
		return 3;
	if (first_two == 0b10)
		return 4;
	if (first_two == 0b11) {
		uint32_t second_two;
		br.read(second_two, 2);
		if (second_two == 0b00)
			return 5;
		if (second_two == 0b01)
			return 6;
		if (second_two == 0b10)
			return 7;
		// Case: Length > 7
		uint32_t next_four;
		uint32_t N = 1;
		while (true) {
			br.read(next_four, 4);
			if (next_four != 0b1111)
				break;
			++N;
		}
		return next_four + (N * 15 - 7);
	}
	return 0;
}

void lzs_decompress(std::istream &is, std::ostream &os) {
	uint32_t bit;
	bitreader br(is);

	std::string decoded;
	std::vector<uint32_t> dictionary;
	
	// Read 1 bit at time 
	while (br.read(bit, 1)) {
		if (bit == 0) {
			uint32_t next;
			br.read(next, 8);
			decoded += static_cast<char>(next);
		}
		// Case bit = 1 -> Offset / Length
		else {
			uint32_t offset, length;
			br.read(bit, 1);
			// Offset < 128 -> Offset has a value of 7 bits
			if (bit == 1) {
				br.read(offset, 7);
				// End Marker
				if (offset == 0)
					break;
			}
			// Offset > 128 -> Offset has a value of 11 bits
			else {
				br.read(offset, 11);
			}
			length = read_length(br);
			for (uint32_t l = 0; l < length; ++l) {
				decoded += static_cast<char>(*(dictionary.end() - offset));
			}

		}
		os << decoded;
		for(const auto &x: decoded)
			dictionary.push_back((uint32_t)x);
		decoded.clear();
	}
}