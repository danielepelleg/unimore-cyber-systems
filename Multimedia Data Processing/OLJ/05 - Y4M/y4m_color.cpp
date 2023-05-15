#include <fstream>
#include <iostream>
#include <vector>
#include <string>

#include "types.h"
#include "mat.h"

double upsample(const mat<uint8_t> &img, int row, int col) {
	return img(row / 2, col / 2);
}

bool y4m_extract_color(const std::string& filename, std::vector<mat<vec3b>>& frames) {
	std::ifstream is(filename, std::ios::binary);
	if (!is){
		return false;
	}

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

	if (chroma_subsampling != "420jpeg" || interlacing != 'p') {
		std::cerr << "Header Error" << std::endl;
		return false;
	}

	if (!is || c != '\n'){
		return false;
	}

	mat<vec3b> frame(height, width);
	mat<uint8_t> Y(height, width);
	mat<uint8_t> Cb(height / 2, width / 2);
	mat<uint8_t> Cr(height / 2, width / 2);

	// Read Frames
	while (true) {
		std::string frame_header;
		is >> frame_header;
		if (frame_header != "FRAME") {
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

		// YCbCr -> RGB
		for (int r = 0; r < frame.rows(); ++r) {
			for (int c = 0; c < frame.cols(); ++c) {
				// Compute sampling
				double y = Y(r, c);
				double cb = upsample(Cb, r, c);
				double cr = upsample(Cr, r, c);

				y = y < 16 ? 16 : y > 235 ? 235 : y;
				cb = cb < 16 ? 16 : cb > 240 ? 240 : cb;
				cr = cr < 16 ? 16 : cr > 240 ? 240 : cr;

				// Compute Coefficients
				y -= 16;
				cb -= 128;
				cr -= 128;

				double R = 1.164*y + 0.000*cb + 1.596*cr;
				double G = 1.164*y - 0.392*cb - 0.813*cr;
				double B = 1.164*y + 2.017*cb + 0.000*cr;

				R = R < 0 ? 0 : R > 255 ? 255 : R;
				G = G < 0 ? 0 : G > 255 ? 255 : G;
				B = B < 0 ? 0 : B > 255 ? 255 : B;

				frame(r, c) = { uint8_t(R), uint8_t(G), uint8_t(B) };
			}
		}
		frames.push_back(frame);
	}
	return true;
}