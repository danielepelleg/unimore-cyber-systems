#include <cstdint>
#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <string>
#include <sstream>

void syntax() {
	std::cout << "\n\tSintassi del programma qoi_decomp:"
		<< "\n\tqoi_decomp <input file> <output file>\n\n";
	exit(EXIT_FAILURE);
}

void error(const std::string& errorMessage) {
	std::cout << "\n\tERRORE: " + errorMessage + "\n\n";
	exit(EXIT_FAILURE);
}

struct bitreader {

	uint8_t buffer_;
	uint8_t nbits_ = 0;
	std::istream& is_;

	bitreader(std::istream& is) : is_(is) {};

	int read_bit() {
		if (nbits_ == 0) {
			if (!is_.read(reinterpret_cast<char*>(&buffer_), 1))
				return EOF;
			nbits_ = 8;
		}
		--nbits_;
		return (buffer_ >> nbits_) & 1;
	}

	explicit operator bool() { return bool(is_); }
	bool operator!() { return !is_; }

	std::istream& read(uint8_t& u, uint8_t n) {
		u = 0;
		while (n-- > 0) {
			u = (u << 1) | read_bit();
		}
		return is_;
	}
};

template<typename T>
struct mat {
	size_t rows_, cols_;
	std::vector<T> data_;

	mat(size_t rows, size_t cols) : rows_(rows), cols_(cols), data_(rows*cols) {}

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

struct qoi_header {
	char magic[4]; // magic bytes "qoif"
	uint32_t width; // image width in pixels (BE)
	uint32_t height; // image height in pixels (BE)
	uint8_t channels; // 3 = RGB, 4 = RGBA
	uint8_t colorspace; // 0 = sRGB with linear alpha
						// 1 = all channels linear
};

struct pixel {
	uint8_t R_, G_, B_, A_;
	pixel() : R_(0), G_(0), B_(0), A_(255) {};
	pixel(uint8_t R, uint8_t G, uint8_t B, uint8_t A) : R_(R), G_(G), B_(B), A_(A) {};
	size_t getIndexPosition() {
		return  (R_ * 3 + G_ * 5 + B_ * 7 + A_ * 11) % 64;
	}
};

struct QOIfile {

	qoi_header header;
	pixel array[64];
	std::istream& is_;

	QOIfile(std::istream& is) : is_(is) { memset(array, 0, sizeof(array)); };

	void headerReader() { // DA SISTEMARE
		is_.read(reinterpret_cast<char*>(&header.magic), sizeof(char[4])); // magic
		header.width = readBEuint32_t(is_);
		header.height = readBEuint32_t(is_);
		is_.read(reinterpret_cast<char*>(&header.channels), 1);
		is_.read(reinterpret_cast<char*>(&header.colorspace), 1);
	}

	void decodeData(mat<pixel>& img) {
		pixel previousPixel(0, 0, 0, 255), pixelRead;
		uint8_t run = 1;
		size_t matrixIndex = 0;
		bitreader br(is_);
		uint8_t valRead;
		while (is_.read(reinterpret_cast<char*>(&valRead), 1)) {

			if (checkForEndQOIfile(valRead))
				break;

			// Sposto il puntatore indietro di una posizione
			is_.seekg((size_t)is_.tellg() - 1);

			if (valRead == 0b11111110) { // QOI_OP_RGB
				pixelRead = readQOI_OP_RGB(); // Leggo il pixel
			}
			if (valRead == 0b11111111) { // QOI_OP_RGBA
				pixelRead = readQOI_OP_RGBA(); // Leggo il pixel
			}
			if (valRead <= 0b00111111) { // QOI_OP_INDEX 
				pixelRead = readQOI_OP_INDEX(br); // Leggo il pixel
			}
			if (valRead > 0b00111111 && valRead <= 0b01111111) { // QOI_OP_DIFF
				pixelRead = readQOI_OP_DIFF(previousPixel, br); // Leggo il pixel
			}
			if (valRead > 0b01111111 && valRead <= 0b10111111) { // QOI_OP_LUMA
				pixelRead = readQOI_OP_LUMA(previousPixel, br); // Leggo il pixel
			}
			if (valRead > 0b10111111 && valRead < 0b11111110) { // QOI_OP_RUN
				pixelRead = previousPixel; // Carico il pixel precedente
				run = readQOI_OP_RUN(br); // Leggo la run
			}

			// Operazioni comuni
			array[pixelRead.getIndexPosition()] = pixelRead; // Lo inserisco nell'array
			for (uint8_t n = 0; n < run; ++n) { // Lo riporto run volte nell'immagine
				img[matrixIndex] = pixelRead;
				++matrixIndex;
			}
			run = 1; // Resetto la run a 1
			previousPixel = pixelRead; // Mi predispongo all'iterazione successiva
		}
	}

	pixel readQOI_OP_RGB() {
		uint8_t tag;
		pixel p;
		is_.read(reinterpret_cast<char*>(&tag), 1); // TAG
		is_.read(reinterpret_cast<char*>(&p.R_), 1); // R
		is_.read(reinterpret_cast<char*>(&p.G_), 1); // G
		is_.read(reinterpret_cast<char*>(&p.B_), 1); // B
		return p;
	}

	pixel readQOI_OP_RGBA() {
		uint8_t tag;
		pixel p;
		is_.read(reinterpret_cast<char*>(&tag), 1); // TAG
		is_.read(reinterpret_cast<char*>(&p.R_), 1); // R
		is_.read(reinterpret_cast<char*>(&p.G_), 1); // G
		is_.read(reinterpret_cast<char*>(&p.B_), 1); // B
		is_.read(reinterpret_cast<char*>(&p.A_), 1); // A
		return p;
	}

	pixel readQOI_OP_INDEX(bitreader &br) {
		uint8_t tag, index;
		br.read(tag, 2);
		br.read(index, 6);
		return array[index];
	}

	pixel readQOI_OP_DIFF(pixel previousPixel, bitreader& br) {
		uint8_t tag, dr, dg, db;
		pixel p;
		br.read(tag, 2);
		br.read(dr, 2);
		br.read(dg, 2);
		br.read(db, 2);
		p.R_ = wrapAround(dr - 2, previousPixel.R_);
		p.G_ = wrapAround(dg - 2, previousPixel.G_);
		p.B_ = wrapAround(db - 2, previousPixel.B_);
		p.A_ = previousPixel.A_; // Alpha unchanged
		return p;
	}

	pixel readQOI_OP_LUMA(pixel previousPixel, bitreader& br) {
		uint8_t tag, diff_green, dr_dg, db_dg;
		pixel p;
		br.read(tag, 2);
		br.read(diff_green, 6);
		br.read(dr_dg, 4);
		br.read(db_dg, 4);
		p.R_ = wrapAround(dr_dg - 8, previousPixel.R_ + diff_green - 32);
		p.G_ = diff_green - 32 + previousPixel.G_;
		p.B_ = wrapAround(db_dg - 8, previousPixel.B_ + diff_green - 32);
		p.A_ = previousPixel.A_; // Alpha unchanged
		return p;
	}

	uint8_t readQOI_OP_RUN(bitreader& br) {
		uint8_t tag, run;
		br.read(tag, 2);
		br.read(run, 6);
		return run + 1;
	}

	uint8_t wrapAround(int first, int second) {
		if (first + second < 0) { // Difetto
			return first + second + 256;
		}
		if (first + second > 255) { // Eccesso
			return first + second - 256;
		}
		return first + second; // Non necessito del wraparound
	}

	uint32_t readBEuint32_t(std::istream& is) {
		uint8_t b1, b2, b3, b4;
		uint32_t BEval;
		is_.read(reinterpret_cast<char*>(&b1), 1);
		is_.read(reinterpret_cast<char*>(&b2), 1);
		is_.read(reinterpret_cast<char*>(&b3), 1);
		is_.read(reinterpret_cast<char*>(&b4), 1);
		std::stringstream stream;
		stream.write(reinterpret_cast<char*>(&b4), 1);
		stream.write(reinterpret_cast<char*>(&b3), 1);
		stream.write(reinterpret_cast<char*>(&b2), 1);
		stream.write(reinterpret_cast<char*>(&b1), 1);
		stream.read(reinterpret_cast<char*>(&BEval), sizeof(uint32_t));
		return BEval;
	}

	bool checkForEndQOIfile(uint8_t valRead) {
		uint8_t b1, b2, b3, b4, b5, b6, b7, b8;
		is_.read(reinterpret_cast<char*>(&b1), 1);
		is_.read(reinterpret_cast<char*>(&b2), 1);
		is_.read(reinterpret_cast<char*>(&b3), 1);
		is_.read(reinterpret_cast<char*>(&b4), 1);
		is_.read(reinterpret_cast<char*>(&b5), 1);
		is_.read(reinterpret_cast<char*>(&b6), 1);
		is_.read(reinterpret_cast<char*>(&b7), 1);
		is_.read(reinterpret_cast<char*>(&b8), 1);
		is_.seekg((size_t)is_.tellg() - 8);
		if (b1 == 0x00 && b2 == 0x00 && b3 == 0x00 && b4 == 0x00 && b5 == 0x00 && b6 == 0x00 && b7 == 0x00 && b8 == 0x01)
			return true;
		return false;
	}
};
/*
int main(int argc, char *argv[])
{
	// Manage the command line  
	if (argc != 3) {
		syntax();
	}
	std::string inputFilename = argv[1];
	std::string outputFilename = argv[2];
	std::ifstream is(inputFilename, std::ios::binary);
	if (!is) {
		error("Impossibile aprire il file di input " + inputFilename);
	}

	// Lettura dell'header e delle dimensioni dell'immagine
	QOIfile inFile(is);
	inFile.headerReader();
	mat<pixel> img(inFile.header.height, inFile.header.width);

	// Decodificare il file QOI in input e inserire i dati nell'immagine di output
	inFile.decodeData(img);
	// Questo è il formato di output PAM. È già pronto così, ma potete modificarlo se volete
	std::ofstream os(argv[2], std::ios::binary); // Questo utilizza il secondo parametro della linea di comando!
	os << "P7\nWIDTH " << img.cols() << "\nHEIGHT " << img.rows() <<
		"\nDEPTH 4\nMAXVAL 255\nTUPLTYPE RGB_ALPHA\nENDHDR\n";
	os.write(img.rawdata(), img.rawsize());

	return EXIT_SUCCESS;
}

*/