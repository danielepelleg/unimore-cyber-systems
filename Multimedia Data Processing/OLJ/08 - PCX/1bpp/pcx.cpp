#include <fstream>
#include <iostream>
#include <string>
#include <cstdint>
#include <array>

#include "pcx.h"
#include "types.h"
#include "pgm.h"
#include "mat.h"

template<typename T>
std::istream& raw_read(std::istream &is, T &val, size_t size = sizeof(T)) {
	return is.read(reinterpret_cast<char *>(&val), size);
}

bool readHeader(std::istream &is, int &height, int &width, int &total_bytes, int &bytes_to_skip) {
	// Header Names
	uint8_t manufacturer;
	uint8_t version;
	uint8_t encoding;
	uint8_t bits_per_plane;

	uint16_t windowX_min;
	uint16_t windowY_min;
	uint16_t windowX_max;
	uint16_t windowY_max;

	// Dumps
	uint16_t vert_dpi;
	uint16_t horz_dpi;
	uint8_t palette;
	uint8_t reserved;

	uint8_t color_planes;
	uint16_t bytes_per_plane_line;

	// Dumps
	uint16_t palette_info;
	uint16_t hor_scr_size;
	uint16_t ver_scr_size;
	uint8_t padding[54];

	raw_read(is, manufacturer);
	if (manufacturer != 0x0A) {
		std::cerr << "Wrong manufacturer: " << manufacturer << std::endl;
		return false;
	}
	raw_read(is, version);
	if (version != 5) {
		std::cerr << "Wrong version: " << version << std::endl;
		return false;
	}
	raw_read(is, encoding);
	if (encoding != 1) {
		std::cerr << "Wrong encoding: " << encoding << std::endl;
		return false;
	}
	std::cout << "Encoding: " << int(encoding) << "\n";

	raw_read(is, bits_per_plane);
	raw_read(is, windowX_min);
	raw_read(is, windowY_min);
	raw_read(is, windowX_max);
	raw_read(is, windowY_max);

	width = windowX_max - windowX_min + 1;
	height = windowY_max - windowY_min + 1;
	std::cout << "width: " << width << "\nheight: " << height << "\n";

	// Read Dumps
	raw_read(is, vert_dpi);
	raw_read(is, horz_dpi);
	for (int i = 0; i < 48; ++i)
		raw_read(is, palette);
	raw_read(is, reserved);

	raw_read(is, color_planes);
	//int colour_depth = color_planes * bits_per_plane;

	raw_read(is, bytes_per_plane_line);
	total_bytes = color_planes * bytes_per_plane_line;
	std::cout << "Total Bytes: " << total_bytes << std::endl;
	raw_read(is, palette_info);
	raw_read(is, hor_scr_size);
	raw_read(is, ver_scr_size);
	raw_read(is, padding);

	bytes_to_skip = bytes_per_plane_line - width;
	std::cout << bytes_to_skip;
	return true;
}

bool load_pcx(const std::string& filename, mat<uint8_t>& img) {
	std::ifstream is(filename, std::ios::binary);
	if (!is)
		return false;
	int height, int width, int total_bytes, int bytes_to_skip;
	if (!readHeader(is, height, width, total_bytes, bytes_to_skip)) {
		return false;
	}
	img.resize(height, width);

	for (int r = 0; r < img.rows(); r++) {
		read_pcx_line(is, img, r, total_bytes);
	}
	//save_pgm("bunny.pgm", img, true);
	return true;
}

bool load_pcx(const std::string& filename, mat<vec3b>& img) {
	std::ifstream is(filename, std::ios::binary);
	if (!is)
		return false;
	int height, int width, int total_bytes, int bytes_to_skip;
	if (!readHeader(is, height, width, total_bytes, bytes_to_skip)) {
		return false;
	}
	img.resize(height, width);

	for (int r = 0; r < img.rows(); r++) {
		read_pcx_line(is, img, r, total_bytes, bytes_to_skip);
	}
	//save_pgm("bunny.pgm", img, true);
	return true;
}

bool read_pcx_line(std::istream& is, mat<uint8_t>& img, int row, uint32_t total_bytes) {
	uint32_t read_bytes = 0;
	uint32_t col = 0;

	while (read_bytes < total_bytes) {
		uint8_t byte = is.get();
		uint8_t copy = byte & 0x3F;
		// if top two bytes are set
		// copy next bytes n times
		if ((byte & 0xC0) == 0xC0) {
			byte = is.get();
			for (int c = 0; c < copy; ++c) {
				++read_bytes;
				img(row, col++) = ((byte & 0x80) >> 7) * 255;
				img(row, col++) = ((byte & 0x40) >> 6) * 255;
				img(row, col++) = ((byte & 0x20) >> 5) * 255;
				img(row, col++) = ((byte & 0x10) >> 4) * 255;
				img(row, col++) = ((byte & 0x08) >> 3) * 255;
				img(row, col++) = ((byte & 0x04) >> 2) * 255;
				img(row, col++) = ((byte & 0x02) >> 1) * 255;
				img(row, col++) = ((byte & 0x01) >> 0) * 255;
			}
		}
		else {
			++read_bytes;
			img(row, col++) = ((byte & 0x80) >> 7) * 255;
			img(row, col++) = ((byte & 0x40) >> 6) * 255;
			img(row, col++) = ((byte & 0x20) >> 5) * 255;
			img(row, col++) = ((byte & 0x10) >> 4) * 255;
			img(row, col++) = ((byte & 0x08) >> 3) * 255;
			img(row, col++) = ((byte & 0x04) >> 2) * 255;
			img(row, col++) = ((byte & 0x02) >> 1) * 255;
			img(row, col++) = ((byte & 0x01) >> 0) * 255;
		}
	}
	return true;
}

bool read_pcx_line(std::istream& is, mat<vec3b>& img, int row, uint32_t total_bytes, uint32_t skip_bytes) {
	std::vector<uint8_t> buffer;
	uint32_t read_bytes = 0;
	uint32_t c = 0;

	while (read_bytes < total_bytes) {
		uint8_t byte = is.get();
		uint8_t count = byte & 0x3F;
		if ((byte & 0xC0) == 0xC0) {
			byte = is.get();
			for (int i = 0; i < count; i++) {
				buffer.push_back(byte);
				read_bytes++;
			}
		}
		else {
			buffer.push_back(byte);
			read_bytes++;
		}
	}

	int index = 0;
	for (int plane = 0; plane < 3; plane++) {
		for (int c = 0; c < img.cols(); c++) {
			img(row, c)[plane] = buffer[index++];
		}
		index += skip_bytes;
	}

	return true;
}