#include <fstream>
#include <iostream>
#include <array>
#include <iomanip>
#include <iterator>

int error(const char *msg) {
	std::cout << msg;
	return EXIT_FAILURE;
}

void syntax() {
	error("SYNTAX:\n"
		"frequencies <input_file> <output_file>\n"
		"The program takes as input a binary file and for each byte "
		"(interpreted as an unsigned 8-bit integer) it "
		"counts its occurrences.\nThe output is a text file consisting "
		"of one line for each different byte found in the input file");
}

struct frequency_counter {
	std::array<size_t, 256> occurrencies;
	frequency_counter() : occurrencies{ 0 } {}

	void operator()(uint8_t val) {
		++occurrencies[val];
	}

	const size_t& operator[](size_t pos) const {
		return occurrencies[pos];
	}

	size_t& operator[](size_t pos) {
		return occurrencies[pos];
	}

	double entropy() {
		double tot = 0.0;
		for (const auto &x : occurrencies) {
			tot += x;
		}

		double H = 0.0;
		for (const auto &x : occurrencies) {
			if(x > 0){
				double px = x / tot;
				H += px * log2(px);
			}
		}

		return -H;
	}
};

int main(int argc, char *argv[]) {
	using namespace std;

	if (argc != 3) {
		return EXIT_FAILURE;
	}

	ifstream is(argv[1], ios::binary);
	if (!is) {
		error("Cannot open input file\n");
	}
	is.unsetf(ios::skipws);

	uint8_t c;
	frequency_counter f;
	for (istream_iterator<uint8_t> it(is); it != istream_iterator<uint8_t>(); ++it) {
		//cout << &it << endl;
		//cout << *it << endl;
		f(*it);
	}

	ofstream os(argv[2]);
	if (!os) {
		error("Cannot open output file\n");
	}

	for (size_t i = 0; i < 256; i++) {
		if (f[i] > 0) {
			os << setfill('0') << setw(2) << hex << i;
			os << '\t' << f[i] << '\n';
		}
	}

	cout << "Entropy: " << f.entropy() << endl;

	return 0;
}