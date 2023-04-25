#include <iostream>
#include <fstream>

int error(const char *msg) {
	std::cout << msg;
	return EXIT_FAILURE;
}

void syntax() {
	error("SYNTAX:\n"
		"read_int32 <filein.bin> <fileout.txt>\n"
		"The first parameter is the name of a binary file containing 32-bit \n"
		"numbers 2's complement, in little endian. The program must create a \n"
		"new file, with the name passed as the second parameter, with the \n"
		"same numbers saved in decimal text format separated by a new line.\n");
}

template<typename T>
std::istream& raw_read(std::istream &is, T &num, size_t size = sizeof(T)) {
	return is.read(reinterpret_cast<char *>(&num), size);
}

int main(int argc, char *argv[]) {
	using namespace std;
	if (argc != 3) {
		syntax();
	}

	ifstream is(argv[1], ios::binary);
	if (!is) {
		error("Cannot open input file.");
	}

	ofstream os(argv[2]);
	if (!os) {
		error("Cannot open output file.");
	}

	int32_t num;
	while (raw_read(is, num)) {
		os << num << endl;
	}

	return EXIT_SUCCESS;
}