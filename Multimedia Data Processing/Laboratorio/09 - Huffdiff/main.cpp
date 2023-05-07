#include <fstream>
#include <iostream>
#include <array>
#include <string>
#include <vector>
#include <iterator>
#include <climits>
#include <algorithm>
#include <unordered_map>
#include <numeric>
#include <memory>
#include <cstdlib>

#include "bits.h"
#include "freq.h"
#include "pgm.h"

int error(const char *msg) {
	std::cout << msg;
	return EXIT_FAILURE;
}

template<typename T>
struct huffman {
	struct node {
		T sym_; // character byte
		size_t prob_;
		node *left_ = nullptr;
		node *right_ = nullptr;

		node(const T &sym, size_t prob) : sym_(sym), prob_(prob) {}

		node(node *a, node *b) {
			prob_ = a->prob_ + b->prob_;
			left_ = a;
			right_ = b;
		}
	};

	struct code {
		uint32_t len_, val_;

		bool operator<(const code &rhs) const {
			return len_ < rhs.len_;
		}
	};

	template<typename mapT>
	huffman(const mapT &map) {
		std::vector<std::unique_ptr<node>> storage;
		std::vector<node*> vec;
		for (const auto &x: map) {
			node* n = new node(x.first, x.second);
			storage.emplace_back(n); // transfert property of the node
			vec.push_back(n);
		}

		std::sort(begin(vec), end(vec),
			[](const node *a, const node *b) {
			return a->prob_ > b->prob_;
		});

		while (vec.size() > 1) {
			//Take the two least proabble nodes and remove them from the vec
			node *n1 = vec.back();
			vec.pop_back();
			node *n2 = vec.back();
			vec.pop_back();

			//Combine them in a new node
			node *n = new node(n1, n2);
			storage.emplace_back(n);
			//Add the new node back into the vector in the correct positionr_bound(begin(vec), end(vec), n,
			auto it = std::lower_bound(begin(vec), end(vec), n,
				[](const node *a, const node *b) {
				return a->prob_ > b->prob_;
			}
			);
			vec.insert(it, n);
		}
		node *root = vec.back();
		compute_codes(root, 0, 0);
	}

	std::unordered_map<uint8_t, code> codes_table_;

	void compute_codes(const node *p, uint32_t len, uint32_t val) {
		if (p->left_ == nullptr) {
			codes_table_[p->sym_] = { len, val }; // leaf
		}
		else {
			compute_codes(p->left_, len + 1, val << 1);
			compute_codes(p->right_, len + 1, val << 1 | 1);
		}
	};
};

struct triplet {
	uint32_t sym_;
	uint32_t len_;
	uint32_t code_;

	bool operator<(const triplet &rhs) const {
		return len_ < rhs.len_;
	}
};

void encode(const std::string &input, const std::string &output) {
	auto img = read(input);
	if (img.empty()) {
		error("Failed to open input image.");
	}

	matrix<int> diff(img.rows(), img.cols());
	int prev = 0;
	for (int r = 0; r < diff.rows(); ++r) {
		for (int c = 0; c < diff.cols(); ++c) {
			diff(r, c) = img(r, c) - prev;
			prev = img(r, c);
		}
		prev = img(r, 0);
	}

	frequency_counter<uint8_t> f;
	f = for_each(diff.begin(), diff.end(), f);
	std::cout << f.entropy();

	huffman<int> h(f);
	std::vector<huffman<int>> canonical(h.codes_table_.begin(), h.codes_table_.end());
	sort(canonical.begin(), canonical.end());

	huffman<int>::code cur = { 0, 0 };
	for (auto &x : canonical) {

	}
}

void decode(const std::string &input, const std::string &output) {
	
}

void syntax() {
	error("SYNTAX:\n"
		"huffdiff [c|d] <input file> <output file>\n");
}


int main(int argc, char* argv[]) {
	using namespace std;
	if (argc != 4) {
		syntax();
	}
	string operation = argv[1];

	if (operation == "c") {
		encode(argv[2], argv[3]);
	}
	else if (operation == "d") {
		decode(argv[2], argv[3]);
	}
	else {
		error("First parameter is wrong");
	}

	return EXIT_SUCCESS;
}