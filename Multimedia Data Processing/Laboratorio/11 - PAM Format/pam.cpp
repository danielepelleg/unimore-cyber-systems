#include<string>
#include<fstream>
#include<iostream>

#include "matrix.h"
#include "pam.h"

bool write_pam(const std::string &filename, const Matrix<uint8_t> &img) {
	std::ofstream os(filename, std::ios::binary);
	if (!os) {
		return false;
	}

	os << "P7\n";
	os << "WIDTH " << img.rows() << "\n";
	os << "HEIGHT " << img.cols() << "\n";
	os << "DEPTH 1\n";
	os << "MAXVAL 255\n";
	os << "TUPLTYPE GRAYSCALE\n";
	os << "ENDHDR\n";

	os.write(img.rawdata(), img.rawsize());
	return true;
}

bool read_pam(const std::string &filename, Matrix<uint8_t> &img) {
	std::ifstream is(filename, std::ios::binary);
	if (!is) {
		return false;
	}
	std::string magic_number;
	is >> magic_number;
	if (magic_number != "P7") {
		return false;
	}

	bool has_width, has_height, has_depth, has_maxval, has_tupltype = false;
	int width, height, depth, maxval;
	std::string tupltype, token;
	while (is >> token) {
		if (token == "WIDTH") {
			is >> width;
			has_width = true;
		}
		else if(token == "HEIGHT") {
			is >> height;
			has_height = true;
		}
		else if (token == "DEPTH") {
			is >> depth;
			has_depth = true;
		}
		else if (token == "MAXVAL") {
			is >> maxval;
			has_maxval = true;
		}
		else if (token == "TUPLTYPE") {
			is >> tupltype;
			has_tupltype = true;
		}
		else if (token == "ENDHDR") {
			if (is.get() != '\n') {
				return false;
			}
			break;
		}
		else {
			std::string dump;
			std::getline(is, dump);
		}
	}

	if (!has_width || !has_height || !has_depth || !has_maxval || !has_tupltype || depth != 1 || maxval != 255 || tupltype != "GRAYSCALE") {
		return false;
	}
	img.resize(width, height);
	return is.read(img.rawdata(), img.rawsize()).good();
}