#include <cstdlib>
#include <string>
#include <iostream>
#include <vector>

#include "matrix.h"
#include "y4m_gray.h"
#include "y4m_color.h"

bool check_file_extension(const std::string &filename, const std::string &extension) {
	return filename.substr(filename.size() - extension.size(), extension.size()) == extension;
}

int error(std::string msg) {
	std::cout << msg;
	return EXIT_FAILURE;
}

int main(int argc, char *argv[]) {
	if (argc != 2) {
		error("Syntax error");
	}
	std::string filename = argv[1];
	if (!check_file_extension(filename, ".y4m")) {
		error("The file is not .y4m");
	}

	// Gray Frames
	std::vector<Matrix<uint8_t>> gray_frames;
	y4m_extract_gray(filename, gray_frames);

	// Color Frames
	std::vector<Matrix<vec3b>> color_frames;
	y4m_extract_color(filename, color_frames);

	return EXIT_SUCCESS;
}