#include <string>
#include <iostream>
#include <fstream>
#include <vector> 
#include <cmath>

struct bitwriter {
	std::ostream& os_;
	size_t nbits_;
	uint8_t buffer_;

	bitwriter(std::ostream& os) : os_(os), nbits_(0) {}

	~bitwriter() { flush(); }

	std::ostream& write_bit(uint32_t u) {
		buffer_ = (buffer_ << 1) | (u & 1);
		if (++nbits_ == 8) {
			os_.write(reinterpret_cast<char*>(&buffer_), sizeof(buffer_));
			nbits_ = 0;
		}
		return os_;
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

struct Dictionary_LZ78 {
	std::vector<std::string> dictionary_;
	int maxbits_;

	Dictionary_LZ78(int maxbits) : maxbits_(maxbits) {}

	std::string& operator[](size_t i) { return dictionary_[i]; }
	size_t size() { return dictionary_.size(); }

	size_t indexOf(std::string seq) {
		return (size_t)(std::find(dictionary_.begin(), dictionary_.end(), seq) - dictionary_.begin());
	}

	bool has(std::string seq) {
		if (dictionary_.size() >= pow(2, maxbits_)) {
			dictionary_.clear();
		}
		if (std::find(dictionary_.begin(), dictionary_.end(), seq) == dictionary_.end()) {
			return false;
		}
		return true;
	}

	void add(std::string seq) {
		if (dictionary_.size() >= pow(2, maxbits_)) {
			dictionary_.clear();
		}
		dictionary_.push_back(seq);
	}

};

struct LastMatch {
	size_t value_ = 0;
	void update(Dictionary_LZ78 &dict, std::string &sequence) {
		if (sequence.length() != 1) {
			sequence.pop_back();
			value_ = dict.indexOf(sequence) + 1;
		}
		else value_ = 0;
	}
	void reset() {
		value_ = 0;
	}
};

bool lz78encode(const std::string &input_filename, const std::string &output_filename, int maxbits) {
	std::ifstream is(input_filename, std::ios::binary);
	if (!is) {
		return false;
	}

	std::ofstream os(output_filename, std::ios::binary);
	if (!os) {
		return false;
	}

	if (maxbits < 1 || maxbits > 30) {
		return false;
	}
	os << "LZ78"; // Magic Number
	bitwriter bw(os);
	bw(maxbits, 5);
	uint8_t chRead;
	Dictionary_LZ78 dict(maxbits);
	LastMatch lastMatch;
	std::string currentSequence;

	// Lambda Function to Write a Couple Output
	auto writeCouple = [&](uint8_t symbol, size_t key, size_t nbits) {
		bw((uint32_t)key, nbits);
		bw(symbol, 8);
	};

	// Read 1 Char at Time
	while (is.read(reinterpret_cast<char *>(&chRead), 1)) {
		currentSequence.push_back(chRead);
		if (!dict.has(currentSequence)) {
			dict.add(currentSequence);
			// Update Match
			lastMatch.update(dict, currentSequence);
			writeCouple(chRead, lastMatch.value_, (size_t)ceil(log2(dict.size())));
			currentSequence.clear();
			lastMatch.reset();
		}
	}
	// Flush Last Sequence
	if (!currentSequence.empty()) {
		lastMatch.update(dict, currentSequence);
		writeCouple(chRead, lastMatch.value_, (size_t)ceil(log2(dict.size())));
	}

	return true;
}

int main(int argc, char * argv[]) {
	if (argc != 3) {
		return EXIT_FAILURE;
	}
	std::string input_file = argv[1];
	std::string output_file = argv[2];

	lz78encode(input_file, output_file, 4);
	
	return EXIT_SUCCESS;
}