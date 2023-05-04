#include <cstdio>
#include <cstdlib>
#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <iterator>
#include <bitset>

#include "bit.h"
#include "raw.h"

int error(const std::string msg) {
	std::cout << msg;
	return EXIT_FAILURE;
}

void syntax() {
	error("SYNTAX:\n"
	"elias [c|d] <filein> <fileout>");
}

void encode(std::string const &input, std::string const &output) {
	std::ifstream is(input);
	if (!is) {
		error("Cannot open input file.\n");
	}

	std::vector<int> numbers{std::istream_iterator<int>(is), std::istream_iterator<int>()};
	// Map values to range [1, +inf]
	for (auto &val : numbers) {
		if (val >= 0) {
			val = val * 2 + 1;
		}
		else {
			val = -val * 2;
		}
	}
	
	std::ofstream os(output, std::ios::binary);
	if (!os) {
		error("Cannot open output file.\n");
	}
	bitwriter bw(os);
	for (auto &k : numbers) {
		int nzeros = log2(static_cast<double>(k));
		int ndigits = nzeros + 1;
		bw(k, ndigits * 2 - 1);
		// Print the Encoding Table
		std::bitset<32> bin(k);
		bin = (bin << 32 - nzeros - ndigits);
		std::string binary_code = "";
		for (int u = 0; u < nzeros + ndigits; ++u) {
			binary_code += static_cast<std::bitset<1>>(bin[bin.size() - 1 - u]).to_string();
		}
		std::cout << k << '\t' << binary_code << '\t' << binary_code.size() << std::endl;
	}
	// Output: A6 42 98 E2 04 80
}

void decode(std::string const &input, std::string const &output) {
	std::ifstream is(input, std::ios::binary);
	if (!is) {
		error("Cannot open input file.\n");
	}

	// Vector of Binary Codes
	std::vector<std::bitset<32>> binary_codes;
	std::bitset<32> bin(0);
	
	bool start = true;
	size_t nzeros = 0;
	
	bitreader br(is);
	int32_t val;
	while (br.read(val, 1)) {
		if (start && val == 1) {
			bin |= 1;
			start = false;
			binary_codes.push_back(bin);
			// Reset
			bin = 0;
		}
		else if (val == 0) {
			++nzeros;
		}
		else {
			// Set 1
			bin |= 1;
			br.read(val, nzeros);
			// Binary Shift Left of nzeros
			bin <<= nzeros;
			// Binary OR with val
			bin |= val;
			binary_codes.push_back(bin);
			// Reset
			nzeros = 0;
			bin = 0;
		}
	}

	std::ofstream os(output);
	if (!os) {
		error("Cannot open output file.\n");
	}
	for (auto &b : binary_codes) {
		// Convert binary codes to decimal values
		int32_t decimal_value = b.to_ulong();
		int32_t val;
		// Perform Inverse of Mapping Function
		if (decimal_value % 2 == 0) {
			val = -(decimal_value / 2);
		}
		else {
			val = (decimal_value - 1) / 2;
		}
		os << val << "\n";
	}
}

int main(int argc, char * argv[]) {
	using namespace std;
	if (argc != 4) {
		syntax();
	}
	string operation = argv[1];
	if (operation == "c") {
		encode(argv[2], argv[3]);
	}
	else if (operation == "d") {
		decode(argv[2], argv[3]);
	}
	else {
		error("Not valid command.\n");
	}
	string command = argv[1];
	return EXIT_SUCCESS;
}