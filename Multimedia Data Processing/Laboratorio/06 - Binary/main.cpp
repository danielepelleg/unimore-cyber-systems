#include <fstream>
#include <iostream>
#include <array>
#include <string>
#include <vector>
#include <iterator>
#include <climits>

int error(const char *msg) {
	std::cout << msg;
	return EXIT_FAILURE;
}

struct frequency_counter {
	std::array<size_t, 256> occurrencies;
	frequency_counter() : occurrencies { 0 } {};

	void operator()(uint8_t val) {
		++occurrencies[val];
	}

	const size_t &operator[](size_t pos) const {
		return occurrencies[pos];
	}
	size_t operator[](size_t pos) {
		return occurrencies[pos];
	}

	double entropy() {
		double tot = 0.0;
		for (const auto &x : occurrencies) {
			tot += x;
		}

		double H = 0.0;
		for (const auto &x : occurrencies) {
			if (x > 0) {
				double px = x / tot;
				H += px * log2(px);
			}
		}
		return H;
	}
};

template<typename T>
std::ostream& raw_write(std::ostream &out, const T &val, size_t size = sizeof(T)) {
	return out.write(reinterpret_cast<const char*>(&val), size);
}

template<typename T>
std::istream& raw_read(std::istream &is, T &val, size_t size = sizeof(T)) {
	return is.read(reinterpret_cast<char *>(&val), size);
}

class bitreader {
	std::istream &is_;
	uint8_t buffer_;
	size_t nbits_;

	uint32_t read_bit() {
		if (nbits_ == 0) {
			raw_read(is_, buffer_);
			nbits_ = 8;
		}
		--nbits_;
		return (buffer_ >> nbits_) & 1;
	}

public:
	bitreader(std::istream &is) : is_(is), nbits_(0) {}

	std::istream& read(uint32_t &u, size_t n) {
		u = 0;
		while (n-- > 0) {
			u = (u << 1) | read_bit();
		}
		return is_;
	}

	std::istream& read(int32_t &i, size_t n) {
		uint32_t u;
		read(u, n);
		i = static_cast<int32_t>(u);
		return is_;
	}
};

class bitwriter {
	std::ostream &os_;
	uint8_t buffer_;
	size_t nbits_;

	std::ostream& write_bit(uint32_t u) {
		buffer_ = (buffer_ << 1) | (u & 1);
		if (++nbits_ == 8) {
			raw_write(os_, buffer_);
			nbits_ = 0;
		}
		return os_;
	}

public:
	bitwriter(std::ostream &os) : os_(os), nbits_(0) {}
	~bitwriter() {
		flush();
	}

	std::ostream& write(uint32_t u, size_t n) {
		for (size_t i = 0; i < n; ++i) {
			write_bit(u >> (n - 1 - i));
		}
		return os_;
	}

	void flush(uint32_t u = 0) {
		while (nbits_ > 0) {
			write_bit(u);
		}
	}

	std::ostream& operator()(uint32_t u, size_t n) {
		return write(u, n);
	}
};

void encode(const std::string &input, const std::string &output) {
	std::ifstream is(input);
	if (!is) {
		error("Cannot open input file");
	}

	std::vector<int32_t> v{ std::istream_iterator<int32_t>(is), std::istream_iterator<int32_t>() };
	auto mm = std::minmax_element(v.begin(), v.end());
	int32_t MinValue = *mm.first;
	int32_t MaxValue = *mm.second;
	uint32_t NumValues = v.size();
	size_t range = MaxValue - MinValue + 1;
	size_t num_bits = static_cast<size_t>(ceil(log2(range)));

	std::ofstream os(output, std::ios::binary); 
	if (!os) {
		error("Cannot open output file");
	}
	os << "BIN!";

	bitwriter bw(os);
	bw(MinValue, 32);
	bw(MaxValue, 32);
	bw(NumValues, 32);

	for (const auto &x : v) {
		bw(x - MinValue, num_bits);
	}
}

void decode(const std::string &input, const std::string &output) {
	std::ifstream is(input, std::ios::binary);
	if (!is) {
		error("Cannot open input file.");
	}

	std::string magic(4, ' ');
	raw_read(is, magic[0], 4);
	if (magic != "BIN!") {
		error("Invalid format.\n");
	}

	bitreader br(is);

	int32_t MinValue;
	int32_t MaxValue;
	uint32_t NumValue;

	br.read(MinValue, 32);
	br.read(MaxValue, 32);
	br.read(NumValue, 32);\

	size_t range = MaxValue - MinValue + 1;
	size_t num_bits = static_cast<size_t>(ceil(log2(range)));

	std::ofstream os(output);
	if (!os) {
		error("Cannot open output file.");
	}
	for (uint32_t i = 0; i < NumValue; i++) {
		int32_t val;
		br.read(val, num_bits);
		os << val + MinValue << "\n";
	}
}

void syntax() {
	error("SYNTAX:\n"
		"huffman1 [c|d] <input file> <output file>\n");
}

int main(int argc, char* argv[]) {
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
		error("First parameter is wrong");
	}
	return EXIT_SUCCESS;
}