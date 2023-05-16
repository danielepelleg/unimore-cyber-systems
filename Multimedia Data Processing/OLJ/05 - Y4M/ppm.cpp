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
				os	<< static_cast<int>(img(r, c)[0]) << " "
					<< static_cast<int>(img(r, c)[1]) << " "
					<< static_cast<int>(img(r, c)[2]) << " ";
			}
			os << "\n";
		}
	}
	else {
		os.write(img.rawdata(), img.rawsize());
	}

	return true;
}

static uint8_t get_byte(std::istream& is) 
{
	int val;
	is >> val;
	if (0 <= val && val <= 255)
		return val;
	else
		throw std::range_error("Wrong pixel value");
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

	if (ascii) {
		for (int r = 0; r < img.rows(); ++r) {
			for (int c = 0; c < img.cols(); ++c) {
				try {
					img(r, c)[0] = get_byte(is);
					img(r, c)[1] = get_byte(is);
					img(r, c)[2] = get_byte(is);
				}
				catch (const exception& e) {
					std::cout << e.what() << "\n";
					return false;
				};
			}
		}
	}
	else {
		is.read(img.rawdata(), img.rawsize());
	}

	return true;
}
