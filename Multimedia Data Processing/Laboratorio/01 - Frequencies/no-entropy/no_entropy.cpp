#include <fstream>
#include <iostream>
#include <array>
#include <iomanip>

int error(const char *msg) {
	std::cout << msg;
	return EXIT_FAILURE;
}

int no_entropy(int argc, char *argv[]) {
	// It's ok at function level
	using namespace std;

	if (argc != 3) {
		error("SYNTAX:\n"
			"frequencies <input_file> <output_file>\n"
			"The program takes as input a binary file and for each byte " 
			"(interpreted as an unsigned 8-bit integer) it "
			"counts its occurrences.\nThe output is a text file consisting "
			"of one line for each different byte found in the input file");

		return EXIT_FAILURE;
	}

	/*
	uint8_t my_byte;
	int8_t a;
	int16_t b,
	uint16_t c;
	int32_t d, // int
	uint32_t e;
	int64_t f, // long long
	uint64_t g;

	// type of &my_byte is uint8_t*, meaning  a pointer to uint8_t
	is.read((char *)&my_bye, 1); // C-style cast
	// functio-style cast doesn't exist for pointers
	is.read(char*(&my_byte),1);
	// function style cast that works
	is.read(charptr(&my_byte), 1);
	// C++ style casting

	(double)5; // You change an int to a double === static_cast
	const int x = 5;
	int *p = &x; // can't do it because through p we could modify a const variable
	int *p = (int*) &x; // removing constness === const_cast
	float f = 5.25;
	int *q = (int*)&f; // changin the interpretation of memory address === reinterpret_cast
	is.read(reinterpret_cast<char*>(&my_byte),1); // C++ reinterpret_cast

	At the start my_byte assume a random value like 204. An uninitialized variable has a random content.

	*/
	
	// Open file in binary mode
	std::ifstream is(argv[1], ios::binary);
	if (!is) {
		error("Cannot open input file\n");
	}
	is.unsetf(std::ios::skipws);

	/*
	
	*/

	// from 0 to 255 elements it's 256 elements
	array<size_t, 256> occurencies = { 0 };
	// 'char' is a signed 8 bit integer, so we change it to uint8_t type
	uint8_t c;
	// 'is >> c' escapes white spaces, while is.get(c) get every character
	// it's possible to use 'is >> noskipws >> c'
	// or before using 'is.unsetf(std::ios::skipws);'
	while (is >> c) {
		++occurencies[c];
	}

	ofstream os(argv[2]);
	if (!os) {
		error("Cannot open output file\n");
	}

	for (size_t i = 0; i < 256; i++) {
		if (occurencies[i] > 0) {
			// Exadecimal format with 2 values and fill with 0
			os << setfill('0') << setw(2) << hex << i;
			os << '\t' << occurencies[i] << '\n';
		}
	
	}


	return 0;
}