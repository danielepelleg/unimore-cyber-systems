#include<fstream>

#include "y4m_color.h"

double upsample(const Matrix<uint8_t>& img, int r, int c) {
	return img(r / 2, c / 2);
}

bool y4m_extract_color(const std::string& filename, std::vector<Matrix<vec3b>>& frames) {
	std::ifstream is(filename, std::ios::binary);
	if (!is) {
		return false;
	}
	std::string stream_header;
	is >> stream_header;
	if (stream_header != "YUV4MPEG2") {
		return false;
	}
	int width, height;
	std::string chroma_subsampling = "420jpeg", interlacing = "p";
	char c;
	while (is.get(c) && c == ' ') {
		char tag = is.get();
		switch (tag) {
		case 'W':
			is >> width;
			break;
		case 'H':
			is >> height;
			break;
		case 'I':
			is >> interlacing;
			break;
		case 'C':
			is >> chroma_subsampling;
			break;
		default:
			std::string dump;
			is >> dump;
			break;
		}
	}
	if (!is || c != '\n') {
		return false;
	}

	Matrix<vec3b> frame(height, width);
	Matrix<uint8_t> Y(height, width);
	Matrix<uint8_t> Cb(height / 2, width / 2);
	Matrix<uint8_t> Cr(height / 2, width / 2);
	while (true) {
		std::string frame_header;
		is >> frame_header;
		if (frame_header != "FRAME") {
			break;
		}
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
		// Conversion YCbCr to RGB
		for (int r = 0; r < frame.rows(); ++r) {
			for (int c = 0; c < frame.cols(); ++c) {
				// Compute Sampling;
				double y = Y(r, c);
				double cb = upsample(Cb, r, c);
				double cr = upsample(Cr, r, c);

				// y must be bewtween 16 and 235
				y = y < 16 ? 16 : y > 235 ? 235 : y;
				// Cb and Cr must be between 16 and 240
				cb = cb < 16 ? 16 : cb > 240 ? 240 : cb;
				cr = cr < 16 ? 16 : cr > 240 ? 240 : cr;

				// Comput coefficients
				y -= 16;
				cb -= 128;
				cr -= 128;

				double r = 1.164*y + 0.000*cb + 1.596*cr;
				double g = 1.164*y - 0.392*cb - 0.813*cr;
				double b = 1.164*y + 2.017*cb + 0.000*cr;

				r = r < 0 ? 0 : r > 255 ? 255 : r; 
				g = g < 0 ? 0 : g > 255 ? 255 : g;
				b = b < 0 ? 0 : b > 255 ? 255 : b;

				frame(r, c) = { uint8_t(r), uint8_t(g), uint8_t(b) };
			}
		}
		frames.push_back(frame);

	}
	return true;
}