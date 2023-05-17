#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <string>
#include <functional>
#include <exception>
#include <string>
#include <sstream>

using namespace std;

#include "ppm.h"
#include "image_operations.h"

struct Element {
	std::string label;
};

struct Canvas : Element {
	uint16_t width, height;
	vec3b background;

	Canvas() { label = "canvas"; }
};

struct Figure : Element {
	image<vec3b> data;
	vector<string> features;
	size_t x, y, height, width;

	void printFeatures() {
		cout << this->label << " : ";
		for (auto& feature : features)
			cout << feature << ",";
		cout << "\n";
	}
};

uint16_t read2Bytes(uint8_t first, uint8_t second) {
	stringstream stream;
	string stringContent;
	stream.write(reinterpret_cast<char *>(&first), 1);
	stream.write(reinterpret_cast<char *>(&second), 1);
	uint16_t result = (first << 8) | second;
	return result;
}

Canvas readCanvas(std::istream &is, unsigned &w, unsigned &h) {
	Canvas canvasObj;
	std::string content;
	std::getline(is, content, '}');
	std::cout << content;

	string label = "width";
	size_t pos = content.find(label);
	canvasObj.width = content[pos + label.length() + 1];
	// 2 Byte Case
	if (content[pos + label.length() + 2] != 'i') {
		canvasObj.width = read2Bytes(content[pos + label.length() + 1], content[pos + label.length() + 2]);
	}
	label = "height";
	pos = content.find(label);
	canvasObj.height = content[pos + label.length() + 1];
	// 2 Byte Case
	if (content[pos + label.length() + 2] != 'i') {
		canvasObj.height = read2Bytes(content[pos + label.length() + 1], content[pos + label.length() + 2]);
	}
	string label = "background[$U#i";
	size_t pos = content.find(label);
	canvasObj.background[0] = content[pos + label.length() + 1];
	canvasObj.background[1] = content[pos + label.length() + 2];
	canvasObj.background[2] = content[pos + label.length() + 3];
	w = canvasObj.width;
	h = canvasObj.height;
	return canvasObj;
}

image<vec3b> canvasToImage(Canvas c) {
	image<vec3b> img(c.width, c.height);
	for (auto &rgb : img) {
		rgb = c.background;
	}
	return img;
}

void printFigures(vector<Figure>& objs) {
	for (auto& obj : objs) {
		if (!obj.label.empty())
			obj.printFeatures();
	}
}

void saveImages(vector<Figure>& objs) {
	size_t counter = 1;
	for (auto& figura : objs) {
		if (figura.label == "image") {
			string filename = "image" + to_string(counter);
			filename.append(".ppm");
			writeP6(filename, figura.data);
			++counter;
			break; // Perché a quanto pare non bisogna produrre più di un'immagine. Mah
		}
	}
}

void setStreamToData(istream& is, vector<Figure> objs) {
	string line;
	is.seekg(0);
	size_t nImages = 0;
	for (auto& elem : objs) {
		if (elem.label == "image")
			++nImages;
	}
	size_t counter = 0;
	while (getline(is, line, '[')) {
		if (line.find("data") != string::npos) {
			if (counter == nImages)
				break;
			++counter;
		}
	}
	size_t pos = is.tellg();
	pos = pos + 5;
	is.seekg(pos);
	if (is.peek() == 'z')
		++pos;
	is.seekg(pos);
}

Figure getImageData(istream& is, vector<Figure> objs, string &elementContent) {

	Figure image;
	image.label = "image"; 
	image.features.push_back("x"); 
	image.features.push_back("y"); 
	image.features.push_back("width"); 
	image.features.push_back("height"); 
	image.features.push_back("data");
	// IMG Position
	size_t imageLabelPos = elementContent.find("image");
	// Clear
	elementContent = elementContent.substr(imageLabelPos, elementContent.length() - imageLabelPos);
	size_t xPos = elementContent.find("x"); // x
	image.x = (uint8_t)elementContent[xPos + 2];
	size_t yPos = elementContent.find("y"); // y
	image.y = (uint8_t)elementContent[yPos + 2];
	string label = "width"; // width
	image.width = (uint8_t)elementContent[elementContent.find(label) + label.length() + 1];
	label = "height"; // height
	image.height = (uint8_t)elementContent[elementContent.find(label) + label.length() + 1];
	image.data.resize(image.width, image.height);
	size_t dataPos = elementContent.find("[") + 5; // data
	// Qui la situazione si complica: incappando nel carattere '}' come dato, si rischia di interrompere il flusso di dati.
	// Riapro lo stream e mi setto in prossimità della parentesi '['
	setStreamToData(is, objs);
	for (size_t y = 0; y < image.data.height(); ++y) {
		for (size_t x = 0; x < image.data.width(); ++x) {
			is.read(reinterpret_cast<char*>(&image.data(x, y)[0]), 1);
			is.read(reinterpret_cast<char*>(&image.data(x, y)[1]), 1);
			is.read(reinterpret_cast<char*>(&image.data(x, y)[2]), 1);
		}
	}
	if (is.peek() != '}') {
		setStreamToData(is, objs);
		// Scarto il primo pixel:
		is.read(reinterpret_cast<char*>(&image.data(0, 0)[0]), 1);
		is.read(reinterpret_cast<char*>(&image.data(0, 0)[1]), 1);
		is.read(reinterpret_cast<char*>(&image.data(0, 0)[2]), 1);
		for (size_t y = 0; y < image.data.height(); ++y) {
			for (size_t x = 0; x < image.data.width(); ++x) {
				is.read(reinterpret_cast<char*>(&image.data(x, y)[0]), 1);
				is.read(reinterpret_cast<char*>(&image.data(x, y)[1]), 1);
				is.read(reinterpret_cast<char*>(&image.data(x, y)[2]), 1);
			}
		}
	}
	return image;
}

string getWord(string& elementContent, size_t &i) {
	size_t init = i;
	string word;
	while (isalpha(elementContent[i]) || elementContent[i] == '-') {
		++i;
	}
	word.clear();
	if (init != i)
		word = elementContent.substr(init, i - init);
	return word;
}

vector<Figure> processUBJContent(istream& is) {
	vector<Figure> objs;
	string elementConent;
	while (getline(is, elementConent, '}')) {
		if (elementConent.find("image") != string::npos)
			objs.push_back(getImageData(is, objs, elementConent));
		else objs.push_back(getFigureFeatures(elementConent));
		//if (elementConent.length() < 10) break;
	}
	return objs;
}

Figure getFigureFeatures(string& elementContent) {
	Figure currentFigure;
	if (elementContent.find("width") == string::npos)
		return currentFigure;
	for (size_t i = 0; i < elementContent.length(); ++i) {
		if (elementContent[i] == 'i') {
			i = i + 2; // Salto l'elemento d'indentazione
			string feature = getWord(elementContent, i);
			// Controlli:
			if (!feature.empty() && feature != "elements") {
				if (feature[feature.length() - 1] == 'i')
					feature.pop_back();
				if (currentFigure.label.empty()) currentFigure.label = feature; // Se non ho ancora l'etichetta iniziale, la inserisco
				else currentFigure.features.push_back(feature); // Altrimenti inserisco una feature normale
			}
		}
	}
	return currentFigure;
}

void addImageToCanvas(image<vec3b> &img, vector<Figure>& objs) {
	for (auto& element : objs) {
		if (element.label == "image")
			paste(img, element.data, element.x, element.y);
	}
}

int convert(const string& sInput, const string& sOutput) {
	ifstream is(sInput, std::ios::binary);
	if (!is) {
		return EXIT_FAILURE;
	}
	ofstream os(sOutput, std::ios::binary);
	if (!os) {
		return EXIT_FAILURE;
	}

	unsigned w, h;
	Canvas canvas = readCanvas(is, w, h);
	image<vec3b> canvasImage = canvasToImage(canvas);
	image<vec3b> img(w, h);

	// Per accedere ai pixel di img posso usare img(x,y) oppure img.begin() e img.end()
	paste(img, canvasImage, 0, 0);
	// Dal file UBJ devo estrarre le informazioni sulle immagini da incollare su img 
	vector<Figure> objs = processUBJContent(is);
	// Print
	printFigures(objs);
	// Save PPM
	saveImages(objs);

	// Output in formato PPM
	addImageToCanvas(img, objs);
	if (!writeP6(sOutput, img))
		return EXIT_FAILURE;

	return EXIT_SUCCESS;
}

int main(int argc, char *argv[]) {
	if (argc != 3) {
		return EXIT_FAILURE;
	}
	string sInput = argv[1];
	string sOutput = argv[2];

	return convert(sInput, sOutput);
}