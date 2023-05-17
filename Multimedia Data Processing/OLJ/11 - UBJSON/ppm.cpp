#include "ppm.h"

#include <fstream>
using namespace std;

bool writeP3(const string& sFileName, const image<vec3b>& img) {
	ofstream os(sFileName, ios::binary); 

	// Header
	os << "P3\n";
	os << "#plain PPM creato nel corso di SdEM\n";
	os << img.width() << " " << img.height() << "\n";
	os << "255\n";

	// Dati
	for (unsigned y = 0; y < img.height(); ++y) {
		os << static_cast<int>(img(0, y)[0]) << " " << static_cast<int>(img(0, y)[1]) << " " << static_cast<int>(img(0, y)[2]);
		for (unsigned x = 1; x < img.width(); ++x) {
			os << " " << static_cast<int>(img(x, y)[0]) << " " << static_cast<int>(img(x, y)[1]) << " " << static_cast<int>(img(x, y)[2]);
		}
		os << "\n";
	}

	return true;
}

bool writeP6(const string& sFileName, const image<vec3b>& img) {
	ofstream os(sFileName, ios::binary);

	// Header
	os << "P6\n";
	os << "#PPM creato nel corso di SdEM\n";
	os << img.width() << " " << img.height() << "\n";
	os << "255\n";

	// Dati
	os.write(img.data(), img.data_size());

	return true;
}

bool readP3(const string& sFileName, image<vec3b>& img) {
	ifstream is(sFileName, ios::binary);
	if (!is)
		return false;

	string sMagic;
	if (!getline(is, sMagic) || sMagic != "P3")
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
			img(x, y)[0] = static_cast<mybyte>(val);
			if (!(is >> val) || val>255) 
				return false;
			img(x, y)[1] = static_cast<mybyte>(val);
			if (!(is >> val) || val>255) 
				return false;
			img(x, y)[2] = static_cast<mybyte>(val);
		}
	}

	return true;
}

bool readP6(const string& sFileName, image<vec3b>& img) {
	ifstream is(sFileName, ios::binary);
	if (!is)
		return false;

	string sMagic;
	if (!getline(is, sMagic) || sMagic != "P6")
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