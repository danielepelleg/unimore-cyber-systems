#include <iostream>
#include <fstream>

int error(const char *msg) {
	std::cout << msg;
	return EXIT_FAILURE;
}

void syntax() {
	error("SYNTAX:\n"
		"read_int11 <filein.bin> <fileout.txt>\n"
		"The first parameter is the name of a binary file that contains \n"
		"11-bit numbers in 2’s complement, with the bits sorted \n"
		"from most significant to least significant. \n"
		"The program must create a new file, with the name passed as the second parameter, \n"
		"with the same numbers saved in decimal text format separated by a new line. \n" 
		"Ignore any excess bits in the last byte. \n");
}

class bitreader {
private:
	std::istream &is_;
	uint8_t buffer_;
	size_t nbits_;

	uint32_t read_bit() {
		// If buffer is empty read 1 byte
		if (nbits_ == 0) {
			raw_read(is_, buffer_);
			nbits_ = 8;
		}
		--nbits_;
		return (buffer_ >> nbits_) & 1;
	}

public:
	bitreader(std::istream &is) : is_(is), nbits_(0) {}
	std::istream& read(uint32_t u, size_t n) {
		u = 0;
		while (n --> 0) {
			u = (u << 1) | read_bit();
		}
		return is_;
	}
};

template <typename T>
std::istream& raw_read(std::istream &is, T &val, size_t size=sizeof(T)) {
	return is.read(reinterpret_cast<char *>(&val), size);
}

template <typename T>
std::ostream& raw_write(std::ostream &out, T &val, size_t size=sizeof(T)) {
	return out.write(reinterpret_cast<char *>(&val), size);
}

int main(int argc, char* argv[]) {
	using namespace std;
	if (argc != 3) {
		syntax();
	}
	ifstream is(argv[1], ios::binary);
	if (!is) {
		error("Cannot open input file");
	}

	ofstream os(argv[2]);
	if (!os) {
		error("Cannot open output file");
	}
	bitreader br(is);
	uint32_t unum;
	while (br.read(unum, 11)) {
		int32_t num = unum;
		if (num & 0x400) {
			num -= 0x800;
		}
		os << num << endl;
	}
	return EXIT_SUCCESS;
}