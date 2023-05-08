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
	int width, height;
	std::string chroma_subsampling = "420jpeg";
	char interlacing = 'p';
	char c;
	while (is.get(c) && c == ' ') {
		char tag = is.get();
		switch (tag) {
		case 'W':
			is >> width;
		case 'H':
			is >> height;
		case 'C':
			is >> chroma_subsampling;
		case 'I':
			is >> interlacing;
		default:
			// Ignore other cases
			std::string dump;
			is >> dump;
		}
	}
	if (!is || c != '\n') {
		return false;
	}

	Matrix<uint8_t> Y(height, width);
	Matrix<uint8_t> Cb(height / 2, width / 2);
	Matrix<uint8_t> Cr(height / 2, width / 2);
	// Frames
	while (true) {
		std::string frame_header;
		is >> frame_header;
		if (frame_header != "FRAME") {
			break;
		}
		char c;
		while (is.get(c) || c == ' ') {
			char frame_tag = is.get();
			// Ignore frame tags
			std::string dump;
			is >> dump;
		}

		is.read(Y.rawdata(), Y.rawsize());
		is.read(Cb.rawdata(), Cb.rawsize());
		is.read(Cr.rawdata(), Cr.rawsize());

		if (!is) {
			return false;
		}

		frames.push_back(Y);
	}
	return true;
}