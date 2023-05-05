#pragma once
#include <vector>

template<typename T>
class Matrix {
	int rows_, cols_;
	std::vector<T> data_;

public:
	Matrix(int rows = 0, int cols = 0)
		: rows_(rows), cols_(cols), data_(rows*cols) {}

	// Operators
	T& operator[](int i) { return data_[i]; }
	const T& operator[](int i) const { return data_[i]; }
	T& operator()(int r, int c) { return data_[r * cols_ + c]; }
	const T& operator()(int r, int c) const { return data_[r * cols_ + c]; }

	// Getters
	int rows() const { return rows_; }
	int cols() const { return cols_; }
	int size() const { return rows_ * cols_; }
	auto& data() { return data_;  }
	const auto& data() const { return data_; }
	int rawsize() const { return rows_ * cols_ * sizeof(T); }

	// Raw Data
	auto rawdata() { return reinterpret_cast<char*>(data_.data()); }
	const auto rawdata() const { return reinterpret_cast<const char*>(data_.data()); }

	bool empty() { return rows_ == 0 && cols_ == 0; }
	void resize(int rows, int cols) {
		rows_ = rows;
		cols_ = cols;
		data_.resize(rows*cols);
	}

	auto begin() { return data_.begin(); }
	auto begin() const { return data_.begin(); }
	auto end() { return data_.end(); }
	auto end() const { return data_.end(); }
};