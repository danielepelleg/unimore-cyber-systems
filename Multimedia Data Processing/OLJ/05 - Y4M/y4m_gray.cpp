#include <fstream>
#include <iostream>
#include <vector>
#include <string>

#include "mat.h"

bool y4m_extract_gray(const std::string& filename, std::vector<mat<uint8_t>>& frames) {
	std::ifstream is(filename, std::ios::binary);
	if (!is)
		return false;

	std::string header;
	is >> header;
	if (header != "YUV4MPEG2") {
		std::cerr << "Header Error: " << header << std::endl;
		return false;
	}
	int width, height;
	std::string chroma_subsampling = "420jpeg";
	char interlacing = 'p';

	char c;
	// TAG Reading
	while (is.get(c) && c == ' ') {
		char tag = is.get();
		switch (tag) {
		case 'W':
			is >> width;
			break;
		case 'H':
			is >> height;
			break;
		case 'C':
			is >> chroma_subsampling;
			break;
		case 'I':
			is >> interlacing;
			break;
		default:
			// Ignore these tags
			std::string dumps;
			is >> dumps;
			break;
		}
	}

	if (!is || c != '\n')
		return false;

	mat<uint8_t> Y(height, width);
	mat<uint8_t> Cb(height/2, width/2);
	mat<uint8_t> Cr(height/2, width/2);

	// Read Frames
	while (true) {
		std::string frame_header;
		is >> frame_header;
		if(frame_header != "FRAME") {
			break;
		}

		// Ignore these tags
		char c;
		while (is.get(c) && c == ' ') {
			char frame_tag = is.get();
			std::string dump;
			is >> dump;
		}

		if (!is || c != '\n') {
			return false;
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
