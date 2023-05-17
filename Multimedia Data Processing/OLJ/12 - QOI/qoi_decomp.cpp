#include <cstdint>
#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <string>

using rgba = std::array<uint8_t, 4>;

template<typename T>
struct mat {
	size_t rows_, cols_;
	std::vector<T> data_;

	mat(size_t rows, size_t cols) : rows_(rows), cols_(cols), data_(rows*cols) {}

	void resize(int r, int c) {
		rows_ = r;
		cols_ = c;
		data_.resize(r*c);
	}

	const T& operator[](size_t i) const { return data_[i]; }
	T& operator[](size_t i) { return data_[i]; }

	size_t size() const { return rows_ * cols_; }
	size_t rows() const { return rows_; }
	size_t cols() const { return cols_; }

	const char* rawdata() const {
		return reinterpret_cast<const char*>(data_.data());
	}
	size_t rawsize() const { return size() * sizeof(T); }
};

uint32_t LEtoBE(std::istream &is) {
	uint8_t first, second, third, fourth;
	is.read(reinterpret_cast<char *>(&first), 1);
	is.read(reinterpret_cast<char *>(&second), 1);
	is.read(reinterpret_cast<char *>(&third), 1);
	is.read(reinterpret_cast<char *>(&fourth), 1);
	uint32_t result = (first << 24) | (second << 16) | (third << 8) | fourth;
	return result;
}

struct qoi_header {
	char magic[4]; // magic bytes "qoif"
	uint32_t width; // image width in pixels (BE)
	uint32_t height; // image height in pixels (BE)
	uint8_t channels; // 3 = RGB, 4 = RGBA
	uint8_t colorspace; // 0 = sRGB with linear alpha

	void print() {
		std::string magic_number(magic, 4);
		std::cout << "Magic Number: " << magic_number << std::endl;
		std::cout << "Width: " << width << std::endl;
		std::cout << "Height: " << height << std::endl;
		std::cout << "Channels : " << (int)channels << std::endl;
		std::cout << "Colorspace: " << (int)colorspace << std::endl;
	}
};

qoi_header readQoiHeader(std::istream &is) {
	qoi_header QOI_header;

	is.read(reinterpret_cast<char *>(&QOI_header.magic), 4);
	QOI_header.width = LEtoBE(is);
	QOI_header.height = LEtoBE(is);
	is.read(reinterpret_cast<char *>(&QOI_header.channels), 1);
	is.read(reinterpret_cast<char *>(&QOI_header.colorspace), 1);

	QOI_header.print();

	return QOI_header;
}

struct Pixel {
	uint8_t R_, G_, B_, A_;
	Pixel() : R_(0), G_(0), B_(0), A_(255) {};
	Pixel(uint8_t R, uint8_t G, uint8_t B, uint8_t A) : R_(R), G_(G), B_(B), A_(A) {};
	size_t getIndexPosition() {
		return  (R_ * 3 + G_ * 5 + B_ * 7 + A_ * 11) % 64;
	}
};

Pixel read_QOI_OP_RGB(std::istream &is) {
	Pixel p;
	is.read(reinterpret_cast<char *>(&p.R_), 1);
	is.read(reinterpret_cast<char *>(&p.G_), 1);
	is.read(reinterpret_cast<char *>(&p.B_), 1);
	return p;
}

Pixel read_QOI_OP_RGBA(std::istream &is) {
	Pixel p;
	is.read(reinterpret_cast<char *>(&p.R_), 1);
	is.read(reinterpret_cast<char *>(&p.G_), 1);
	is.read(reinterpret_cast<char *>(&p.B_), 1);
	is.read(reinterpret_cast<char *>(&p.A_), 1);
	return p;
}

uint8_t wrapAround(int first, int second) {
	if (first + second < 0) {
		return first + second + 256;
	}
	if (first + second > 255) {
		return first + second - 256;
	}
	return first + second;
}

Pixel read_QOI_OP_DIFF(uint8_t tag, Pixel prev) {
	uint8_t dr, dg, db;
	Pixel p;
	dr = (tag >> 4) & 0b11;
	dg = (tag >> 2) & 0b11;
	db = tag & 0b11;

	p.R_ = wrapAround(dr - 2, prev.R_);
	p.G_ = wrapAround(dg - 2, prev.G_);
	p.B_ = wrapAround(db - 2, prev.B_);
	p.A_ = prev.A_;
	return p;
}

Pixel read_QOI_OP_LUMA(uint8_t tag, uint8_t next_tag, Pixel prev) {
	uint8_t diff_green, dr_dg, db_dg;
	Pixel p;
	diff_green = tag & 0b00111111;
	dr_dg = next_tag >> 4;
	db_dg = next_tag & 0b1111;

	p.R_ = wrapAround(dr_dg - 8, prev.R_ + diff_green - 32);
	p.G_ = diff_green - 32 + prev.G_;
	p.B_ = wrapAround(db_dg - 8, prev.B_ + diff_green - 32);
	p.A_ = prev.A_;
	return p;
}

bool checkEOS(std::istream &is, uint8_t tag) {
	if (tag != 0) {
		return false;
	}
	uint8_t b1, b2, b3, b4, b5, b6, b7;
	is.read(reinterpret_cast<char*>(&b1), 1);
	is.read(reinterpret_cast<char*>(&b2), 1);
	is.read(reinterpret_cast<char*>(&b3), 1);
	is.read(reinterpret_cast<char*>(&b4), 1);
	is.read(reinterpret_cast<char*>(&b5), 1);
	is.read(reinterpret_cast<char*>(&b6), 1);
	is.read(reinterpret_cast<char*>(&b7), 1);
	is.seekg((size_t)is.tellg() - 7);
	if (b1 == 0x00 && b2 == 0x00 && b3 == 0x00 && b4 == 0x00 && b5 == 0x00 && b6 == 0x00 && b7 == 0x01)
		return true;
	return false;

}

mat<rgba> readQoiData(std::istream &is, mat<rgba> &img) {
	Pixel previous_pixel(0, 0, 0, 255);
	Pixel readPixel;
	// Times to Repeat Prev.Pixel
	uint8_t run = 1;
	// IMG Index
	size_t img_index = 0;

	Pixel previously_seen[64];
	uint8_t tag;
	// Interpret TAG
	while (is.read(reinterpret_cast<char *>(&tag), 1)) {

		if (checkEOS(is, tag)) {
			break;
		}

		// Check 8 bits TAG
		else if (tag == 0b11111110) {
			// QOI_OP_RGB
			readPixel = read_QOI_OP_RGB(is);
		}
		else if (tag == 0b11111111) {
			// QOI_OP_RGBA
			readPixel = read_QOI_OP_RGBA(is);
		}
		// Check 2 bits TAG (DATA inside Byte)
		else if ((tag >> 6) == 0b00) {
			// QOI_OP_INDEX
			uint8_t index = (tag & 0b00111111);
			readPixel = previously_seen[index];
		}
		else if ((tag >> 6) == 0b01) {
			// QOI_OP_DIFF
			readPixel = read_QOI_OP_DIFF(tag, previous_pixel);
		}
		else if ((tag >> 6) == 0b10) {
			// QOI_OP_LUMA
			uint8_t next_tag;
			is.read(reinterpret_cast<char *>(&next_tag), 1);
			readPixel = read_QOI_OP_LUMA(tag, next_tag, previous_pixel);
		}
		else if ((tag >> 6) == 0b11 && tag != 0b111110 && tag != 0b111111) {
			// QOI_OP_RUN
			readPixel = previous_pixel;
			run = (tag & 0b111111) + 1;
		}
		// Add Pixel to Prev. Seen Array
		previously_seen[readPixel.getIndexPosition()] = readPixel;
		for (uint8_t n = 0; n < run; ++n) {
			rgba frame = { readPixel.R_, readPixel.G_, readPixel.B_, readPixel.A_ };
			img[img_index] = frame;
			++img_index;
		}
		run = 1;
		previous_pixel = readPixel;
	}
	return img;
}


int main(int argc, char *argv[]) {
	if (argc != 3) {
		return EXIT_FAILURE;
	}

	std::string input_file = argv[1];
	std::string output_file = argv[2];

	std::ifstream is(input_file, std::ios::binary);
	if (!is) {
		return EXIT_FAILURE;
	}
	// TODO: Lettura dell'header e delle dimensioni dell'immagine
	qoi_header QOI_header = readQoiHeader(is);
	mat<rgba> img(QOI_header.height, QOI_header.width);

	// TODO: decodificare il file QOI in input e inserire i dati nell'immagine di output
	img = readQoiData(is, img);

	// Questo è il formato di output PAM. È già pronto così, ma potete modificarlo se volete
	std::ofstream os(argv[2], std::ios::binary);
	os << "P7\nWIDTH " << img.cols() << "\nHEIGHT " << img.rows() <<
		"\nDEPTH 4\nMAXVAL 255\nTUPLTYPE RGB_ALPHA\nENDHDR\n";
	os.write(img.rawdata(), img.rawsize());

	return EXIT_SUCCESS;
}