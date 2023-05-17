#include "pgm.h"

#include <fstream>
using namespace std;

bool writeP2(const string& sFileName, const image<mybyte>& img) {
	ofstream os(sFileName, ios::binary);

	// Header
	os << "P2\n";
	os << "#plain PGM creato nel corso di SdEM\n";
	os << img.width() << " " << img.height() << "\n";
	os << "255\n";

	// Dati
	for (unsigned y = 0; y < img.height(); ++y) {
		os << static_cast<int>(img(0, y));
		for (unsigned x = 1; x < img.width(); ++x) {
			os << " " << static_cast<int>(img(x, y));
		}
		os << "\n";
	}

	return true;
}

bool writeP5(const string& sFileName, const image<mybyte>& img) {
	ofstream os(sFileName, ios::binary);

	// Header
	os << "P5\n";
	os << "#PGM creato nel corso di SdEM\n";
	os << img.width() << " " << img.height() << "\n";
	os << "255\n";

	// Dati
	os.write(img.data(), img.data_size());

	return true;
}

bool readP2(const string& sFileName, image<mybyte>& img) {
	ifstream is(sFileName, ios::binary);
	if (!is)
		return false;

	string sMagic;
	if (!getline(is, sMagic) || sMagic != "P2")
		return false;

	char c = is.get();
	if (c == '#') {
		string sComment;
		if (!getline(is, sComment))
			return false;
	}
	else
		is.putback(c);

	unsigned w, h, max_val;
	if (!(is >> w >> h >> max_val) || max_val != 255)
		return false;

	char lastLF;
	if (!is.get(lastLF) || lastLF!='\n')
		return false;

	img.resize(w, h);
	
	for (unsigned y = 0; y < img.height(); ++y) {
		for (unsigned x = 0; x < img.width(); ++x) {
			unsigned val;
			if (!(is >> val) || val>255) 
				return false;
			img(x, y) = static_cast<mybyte>(val);
		}
	}

	return true;
}

bool readP5(const string& sFileName, image<mybyte>& img) {
	ifstream is(sFileName, ios::binary);
	if (!is)
		return false;

	string sMagic;
	if (!getline(is, sMagic) || sMagic != "P5")
		return false;

	char c = is.get();
	if (c == '#') {
		string sComment;
		if (!getline(is, sComment))
			return false;
	}
	else
		is.putback(c);

	unsigned w, h, max_val;
	if (!(is >> w >> h >> max_val) || max_val != 255)
		return false;

	char lastLF;
	if (!is.get(lastLF) || lastLF != '\n')
		return false;

	img.resize(w, h);

	return is.read(img.data(), img.data_size()).good();
}