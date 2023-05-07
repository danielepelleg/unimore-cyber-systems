#include<fstream>

#include "matrix.h"
#include "y4m_gray.h"

bool y4m_extract_gray(const std::string &filename, std::vector<Matrix<uint8_t>> &frames) {
	std::ifstream is(filename, std::ios::binary);
	if (!is) {
		return false;
	}

	std::string header;
	is >> header;
	if (header != "YUV4MPEG2") {
		return false;
	}	

	return true;
}