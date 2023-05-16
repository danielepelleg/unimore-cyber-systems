#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

#include "base64.h"

int main(int argc, char * argv[]) {
	if (argc != 2)
		return EXIT_FAILURE;
	std::string input = argv[1];
	// Apertura del file in input
	std::ifstream is(input);

	if (!is) {
		std::cerr << "Errore nell'apertura del file: " << input << std::endl;
	}

	// Caricamento del contenuto del file in una stringa
	std::stringstream buffer;
	buffer << is.rdbuf();
	std::string text = buffer.str();

	std::string output = base64_decode(text);
	std::cout << output;

	return EXIT_SUCCESS;
}