#include<iostream>
#include<cstdlib>
#include<string>

int error(const std::string msg) {
	std::cout << msg;
	return EXIT_FAILURE;
}

bool check_extension(const std::string &filename, const std::string &extension) {
	return filename.substr(filename.size() - extension.size(), extension.size()) == extension;
}

int main(int argc, char * argv[]) {
	if (!check_extension(argv[1], ".y4m")) {
		error("File must be a .y4m file");
	}
	return EXIT_SUCCESS;
}