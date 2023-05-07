#include<string>
#include<cstdlib>
#include<iostream>
#include<fstream>
#include<string>

#include "torrent.h"

int error(std::string msg) {
	std::cout << msg;
	return EXIT_FAILURE;
}

bool check_extension(const std::string &filename, const std::string &extension) {
	return filename.substr(filename.size() - extension.size(), extension.size()) == extension;
}

void syntax() {
	error("torrent_dump <file .torrent>\n");
}

int main(int argc, char * argv[]) {
	if (argc != 2) {
		syntax();
	}
	if (!check_extension(argv[1], ".torrent")) {
		error("File is not '.torrent'");
	}
	std::ifstream is(argv[1], std::ios::binary);
	if (!is) {
		error("Cannot open input file.\n");
	}
	Item* elem = decode(is);
	elem->print();
	return EXIT_SUCCESS;
}