#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>
#include <iterator>
#include <vector>

int error(const std::string msg) {
	std::cout << msg;
	return EXIT_FAILURE;
}

void syntax() {
	error("SYNTAX:\n"
	"write_int32 <filein.txt> <fileout.bin>\n"
	"The first parameter is the name of a text file that contains \n"
	"signed base 10 integers from - 1000 to 1000 separated by whitespace. \n"
	"The program must create a new file, with the name passed as the second parameter, \n"
	"with the same numbers saved as 32-bit binary little-endian numbers in 2's complement.");
}

/*
	Conversion char (1 Byte) to int32_t little endian (hex format)
	0000.0000 0000.0000 0000.0000 0111.1000 -> 78.00.00.00
*/
template<typename T>
std::ostream& raw_write(std::ostream &out, const T &num, size_t size = sizeof(T)) {
	return out.write(reinterpret_cast<const char *>(&num), size);
}

int main(int argc, char *argv[]) {
	using namespace std;
	if (argc != 3) {
		syntax();
	}

	ifstream is(argv[1]);
	if (!is) {
		error("Cannot open input file.");
	}

	ofstream os(argv[2], ios::binary);
	if (!os) {
		error("Cannot open output file.");
	}

	int32_t num;
	while (is >> num) {
		raw_write(os, num);
	}

	return EXIT_SUCCESS;
}
