#pragma once

#include <fstream>
#include <vector>

template<typename T>
std::istream& raw_read(std::istream &is, T &val, size_t size = sizeof(T)) {
	return is.read(reinterpret_cast<char *>(&val), size);
}

template<typename T>
std::ostream& raw_write(std::ostream &os, const T &val, size_t size = sizeof(T)) {
	return os.write(reinterpret_cast<const char *>(&val), size);
}

// Samples as signed 16 bit integers (int16_t) 
void write_samples(const std::string &filename, std::vector<int16_t> &samples) {
	std::ofstream os(filename, std::ios::binary);
	for (const auto &s : samples) {
		raw_write(os, s);
	}
}

auto read_samples(const std::string &filename) {
	std::ifstream is(filename, std::ios::binary);
	std::vector<int16_t> samples;
	int16_t sample;
	while (raw_read(is, sample)) {
		samples.push_back(sample);
	}
	return samples;
}