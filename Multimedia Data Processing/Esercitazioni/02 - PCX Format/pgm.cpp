#include "pgm.h"

#include <fstream>

bool save_pgm(const std::string& filename, const mat<uint8_t>& img, bool ascii)
{
	std::ofstream os(filename, std::ios::binary);
	if (!os)
		return false;

	os << (ascii ? "P2" : "P5") << "\n";
	os << "# MDP2020\n";
	os << img.cols() << " " << img.rows() << "\n";
	os << "255\n";

	if (ascii) {
		for (int r = 0; r < img.rows(); ++r) {
			for (int c = 0; c < img.cols(); ++c) {
				os << static_cast<int>(img(r, c)) << " ";
			}
			os << "\n";
		}
		// Alternativa
		//std::copy(std::begin(img), std::end(img), std::ostream_iterator<int>(os, " "));
	}
	else {
		for (int r = 0; r < img.rows(); ++r) {
			for (int c = 0; c < img.cols(); ++c) {
				os.put(img(r, c));
			}
		}
		// Alternativa
		//os.write(img.rawdata(), img.rawsize());
	}

	return true;
}

bool load_pgm(const std::string& filename, mat<uint8_t>& img)
{
	using namespace std;
	ifstream is(filename, ios::binary);
	if (!is)
		return false;

	string MagicNumber;
	is >> MagicNumber;
	bool ascii;
	if (MagicNumber == "P2")
		ascii = true;
	else if (MagicNumber == "P5")
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
				int val;
				is >> val;
				if (0 <= val && val <= 255)
					img(r, c) = val;
				else
					return false;
			}
		}
		// Alternativa
		//copy(istream_iterator<int>(is), istream_iterator<int>(), begin(img));
	}
	else {
		for (int r = 0; r < img.rows(); ++r) {
			for (int c = 0; c < img.cols(); ++c) {
				img(r, c) = is.get();
			}
		}
		// Alternativa
		// is.read(img.rawdata(), img.rawsize());
	}

	return true;
}
