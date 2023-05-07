#pragma once
#include<string>
#include<iostream>
#include<fstream>
#include<memory>
#include<vector>
#include<map>
#include<iomanip>

class Item {
protected:
	/*
	static bool ReadUpToX(std::istream &is, std::string &data, char x) {
		while (is.peek() != std::char_traits<char>::eof() && is.peek() != x) {
			data += is.get();
		}
		return is && is.peek() == x;
	}
	*/
public:
	virtual void print(int tabs = 0) const = 0;
};
// Decide what type of Item Read from the File
Item* decode(std::istream &input, const std::string &key = "");

class IntItem : public Item {
	uint64_t value_;
public:
	IntItem(std::istream &is) {
		char header;
		if (!is.get(header) || header != 'i') {
			throw std::runtime_error("integer header");
		}
		is >> value_;
		char end;
		if (!is.get(end) || end != 'e') {
			throw std::runtime_error("integer end");
		}
	}

	void print(int tabs) const override {
		std::cout << value_;
	}
};

class StringItem : public Item {
public:
	std::string value_;

	StringItem(std::istream &is) {
		size_t len;
		is >> len;
		char colon;
		if (!is.get(colon) || colon != ':') {
			throw std::runtime_error("string colon");
		}
		value_.resize(len);
		if (!is.read(&value_[0], len)) {
			throw std::runtime_error("string content");
		}
	}
	void print(int tabs) const override {
		std::cout << "\"";
		for (const auto &c : value_) {
			if (c < 32 || c > 126) {
				std::cout << ".";
			}
			else {
				std::cout << c;
			}
		}
		std::cout << "\"";
	}
};

class ListItem : public Item {
	std::vector<Item*> value_;
public:
	ListItem(std::istream &is) {
		char header;
		if (!is.get(header) || header != 'l') {
			std::runtime_error("list header");
		}
		while (true) {
			if (!is || is.peek() == 'e') {
				break;
			}
			value_.push_back(decode(is));
		}
		char end;
		if (!is.get(end) || end != 'e') {
			std::runtime_error("list end");
		}
	}
	void print(int tabs) const override {
		std::cout << "[\n";
		for (const auto &item : value_) {
			std::cout << std::string(tabs + 1, '\t');
			item->print(tabs + 1);
			std::cout << '\n';
		}
		std::cout << std::string(tabs, '\t') << ']';
	}
};

class PiecesItem : public StringItem {
public:
	PiecesItem(std::istream &is) : StringItem(is) {}
	void print(int tabs) const override {
		for (size_t i = 0; i < value_.size(); ++i) {
			if (i % 20 == 0) {
				std::cout << '\n' << std::string(tabs + 1, '\t');
			}
			std::cout << std::hex << std::setw(2) << std::setfill('0') << +static_cast<unsigned char>(value_[i]) << std::dec;
		}
	}
};

class DictItem : public Item {
	std::map<std::string, Item*> value_;
public:
	DictItem(std::istream &is) {
		char header;
		if (!is.get(header) || header != 'd') {
			std::runtime_error("dictionary header");
		}
		while (true) {
			char end = is.peek();
			if (!is || is.peek() == 'e') {
				break;
			}
			// Dictionary always starts with a StringItem
			StringItem key(is);
			value_[key.value_] = decode(is, key.value_);
		}
		char end;
		if (!is.get(end) || end != 'e') {
			std::runtime_error("dictionary end");
		}
	}
	void print(int tabs) const override {
		std::cout << "{\n";
		for (const auto &el : value_) {
			std::cout << std::string(tabs + 1, '\t') << "\"" << el.first << "\" => ";
			el.second->print(tabs + 1);
			std::cout << "\n";
		}
		std::cout << std::string(tabs, '\t') << "}";
	}
};

Item* decode(std::istream &input, const std::string &key) {
	char next = input.peek();
	if (!input) {
		return nullptr;
	}
	if (next == 'i') { // integer
		return new IntItem(input);
	}
	else if (next == 'l') { // list
		return new ListItem(input);
	}
	else if (next == 'd') { // dictionary
		return new DictItem(input);
	}
	else { // string
		if (key == "pieces")
			return new PiecesItem(input);
		else
			return new StringItem(input);
	}
}

