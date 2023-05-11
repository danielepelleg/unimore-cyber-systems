#include "base64.h"

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

std::string base64_decode(const std::string& input) {
	std::string output = "";

	// Apertura del file in input
	std::ifstream is(input);

	if (!is) {
		std::cerr << "Errore nell'apertura del file: " << input  << std::endl;
		return output;
	}

	// Caricamento del contenuto del file in una stringa
	//std::stringstream buffer;
	//buffer << is.rdbuf();
	//std::string text = buffer.str();

	if (text.size() % 4 != 0) {
		text += '=';
	}

	// Array di caratteri che rappresentano i simboli base64
	const std::string base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

	// Funzione per decodificare un singolo carattere codificato in base64
	auto decode_char = [&](char c) {
		if (c == '=') {
			return static_cast<uint8_t>(0);
		}

		std::size_t pos = base64_chars.find(c);
		if (pos == std::string::npos) {
			std::cerr << "Carattere non valido in input: " << c << std::endl;
			return static_cast<uint8_t>(0);
		}

		return static_cast<uint8_t>(pos);
	};

	// Decodifica dei dati in base64
	for (std::size_t i = 0; i < text.size(); i += 4) {
		uint8_t b1 = decode_char(text[i]);
		uint8_t b2 = decode_char(text[i + 1]);
		uint8_t b3 = decode_char(text[i + 2]);
		uint8_t b4 = decode_char(text[i + 3]);

		uint32_t value = (b1 << 18) | (b2 << 12) | (b3 << 6) | b4;

		char c1 = static_cast<char>((value >> 16) & 0xFF);
		char c2 = static_cast<char>((value >> 8) & 0xFF);
		char c3 = static_cast<char>(value & 0xFF);

		output += c1;
		if (b3 != 0) output += c2;
		if (b4 != 0) output += c3;
	}
	std::string result = "";
	for (int i = 0; i < output.size(); i++) {
		if (i % 76 == 0)
			result += '\n';
		result += output[i];
	}
	std::cout << result;
	return result;
}

