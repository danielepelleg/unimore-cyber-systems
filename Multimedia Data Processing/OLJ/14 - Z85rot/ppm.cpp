#include "ppm.h"

#include <iostream>
#include <fstream>
#include <exception>

bool save_ppm(const std::string& filename, const mat<vec3b>& img, bool ascii)
{
	std::ofstream os(filename, std::ios::binary);
	if (!os)
		return false;

	os << (ascii ? "P3" : "P6") << "\n";
	os << "# MDP2020\n";
	os << img.cols() << " " << img.rows() << "\n";
	os << "255\n";

	if (ascii) {
		for (int r = 0; r < img.rows(); ++r) {
			for (int c = 0; c < img.cols(); ++c) {
				const vec3b& pixel = img(r, c);
				os << uint32_t(pixel[0]) << " " << uint32_t(pixel[1]) << " " << uint32_t(pixel[2]) << " ";
			}
			os << "\n";
		}
	}
	else {
		os.write(img.rawdata(), img.rawsize());
	}

	return true;
}

bool load_ppm(const std::string& filename, mat<vec3b>& img)
{
	using namespace std;
	ifstream is(filename, ios::binary);
	if (!is)
		return false;

	string MagicNumber;
	is >> MagicNumber;
	bool ascii;
	if (MagicNumber == "P3")
		ascii = true;
	else if (MagicNumber == "P6")
		ascii = false;
	else
		return false;

	if (is.get() != '\n')
		return false;

	if (is.peek() == '#') {
		string comment;
		getline(is, comment);
	}

	int L, A, MaxVal;
	is >> L;
	if (is.get() != ' ')
		return false;
	is >> A;
	if (is.get() != '\n')
		return false;
	is >> MaxVal;
	if (MaxVal != 255 || is.get() != '\n')
		return false;

	img.resize(A, L);

	if (MagicNumber == "P3") {
		for (auto& pixel : img) {
			uint32_t v;
			for (size_t i = 0; i < 3; ++i) {
				is >> v;
				pixel[i] = v;
			}
		}
	}
	else {
		is.read(img.rawdata(), img.rawsize());
	}

	return true;
}