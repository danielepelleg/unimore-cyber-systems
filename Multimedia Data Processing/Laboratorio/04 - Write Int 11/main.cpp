#include <iostream>
#include <fstream>
#include <algorithm>

int error(const char *msg) {
	std::cout << msg;
	return EXIT_FAILURE;
}

void syntax() {
	error("SYNTAX:\n"
		"write_int11 <filein.txt> <fileout.bin>\n"
		"The first parameter is the name of a text file that contains \n"
		"base 10 integers from - 1000 to 1000 separated by whitespace. \n"
		"The program must create a new file, with the name passed as the \n" 
		"second parameter, with the same numbers saved as 11-bit binary in 2�s complement. \n"
		"The bits are inserted in the file from the most significant to the least significant. \n"
		"The last byte of the file, if incomplete, is filled with bits of 0.\n");
}

template<typename T>
std::ostream& raw_write(std::ostream &out, const T &num, size_t size=sizeof(T)){
	return out.write(reinterpret_cast<const char *>(&num), size);
}

class bitwriter {
	std::ostream &os_;
	uint8_t buffer_;
	size_t nbits_;

	std::ostream& write_bit(uint32_t u) {
		// buffer_ = buffer_ * 2 + u % 2;
		buffer_ = (buffer_ << 1) | (u & 1);
		if (++nbits_ == 8) {
			raw_write(os_, buffer_);
			nbits_ = 0;
		}
		return os_;
	}

public:

	bitwriter(std::ostream &os) : os_(os), nbits_(0) {}
	//	u = 0000.0000 0000.0000 0000.0000 0000.1100
	//		0000.0000 0000.0000 0000.0000 0000.0001
	//
	//	a		0010.0011
	//	b		0001.1111
	//	a | b = 0011.1111
	//  
	//	Zero solo se entrambi i valori lo sono.
	//	Se almeno uno dei valori e' 1 mette 1.

	~bitwriter() {
		flush();
	}

	/*
		Non si vuole sostituire il contenuto del buffer, ma semplicemente inserire un bit spostando
			tutto verso sinistra.
		+---+---+---+---+---+---+---+---+
		| a	| b	| c	| d	| e | f | g | h |
		+---+---+---+---+---+---+---+---+
		+---+---+---+---+---+---+---+---+
		| b	| c	| d	| e	| f | g | h | 1 |
		+---+---+---+---+---+---+---+---+
	*/

	// Usually if we write a byte in to a string we expect to see that 8 bits in that same order

	// Write the n least significant bits of u, from the most significant to the least significant
	// i.e from bit #n-1 to bit #0
	std::ostream& write(uint32_t u, size_t n) {
		//while(n --> 0){
		//	write_bit(u >> n); // writes bit #n
		//}
		// Write nbit starting 
		/*
			Assuming a Buffer of 8 bits has already n bits inside we can shift it by 8-n bits, 
			from the most significant part of u, filling the buffer in 1 shot so we can immediately write

			Then we can write 8 bits at a time

			Writing nth bit, where:
			n=11 -> 100.0000.0000
			n=10 ->  10.0000.0000
			n= 9 ->   1.0000.0000
		*/
		for (size_t i = 0; i < n; ++i) {
			write_bit(u >> (n - 1 - i));
		}
		return os_;
	}

	std::ostream& operator()(uint32_t u, size_t n) {
		return write(u, n);
	}

	void flush(uint32_t u = 0) {
		// if we have bits in the buffer
		while (nbits_ > 0) {
			write_bit(u);
		}
	}
};

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
		error("Cannot open output file");
	}
	bitwriter bw(os);

	int32_t num;
	while (is >> num) {
		bw(num, 11);
	}

	return EXIT_SUCCESS;
}