#pragma once

#include<vector>

template<typename T>
class matrix {
	int rows_, cols_;
	std::vector<T> data_;

public:
	matrix(int rows = 0, int cols = 0)
		: rows_(rows), cols_(cols), data_(rows*cols) {}

	T& operator[](int i) { return data_[i]; }
	const T& operator[](int i) const { return data_[i]; }

	T& operator()(int r, int c) { return data_[r*cols_ + c]; }
	const T& operator()(int r, int c) const { return data_[r*cols_ + c]; }

	bool empty() const {
		return rows_ == 0 && cols_ == 0;
	}

	int rows() const { return rows_; }
	int cols() const { return cols_; }
	int size() const { return rows_ * cols_; }
	int rawsize() const { return rows_ * cols_ * sizeof(T); }

	void resize(int rows, int cols) {
		rows_ = rows;
		cols_ = cols;
		data_.resize(rows*cols);
	}

	// To access the Vector as a Vector of Bytes
	// in order to take those bytes and write them on disk
	char* rawdata() { return reinterpret_cast<char*>(data_.data()); }
	const char* rawdata() const { return reinterpret_cast<const char*>(data_.data()); }

	auto& data() { return data_; }
	const auto& data() const { return data_; }

	auto begin() { return data_.begin(); }
	auto begin() const { return data_.begin(); }
	auto end() { return data_.end(); }
	auto end() const { return data_.end(); }
};