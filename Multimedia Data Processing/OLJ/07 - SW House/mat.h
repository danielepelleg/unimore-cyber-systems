#pragma once

#include <array>
#include <vector>
#include <cassert>
#include <iterator>

template <typename T>
class mat
{
    int rows_, cols_;
    std::vector<T> data_;
public:

    mat(int rows = 0, int cols = 0)
    {
        resize(rows, cols);
    }

    void resize(int rows, int cols)
    {
        assert(rows >= 0 && cols >= 0);
        rows_ = rows;
        cols_ = cols;
        data_.resize(rows*cols);
    }

    int rows() const { return rows_; }
    int cols() const { return cols_; }

    T& operator()(int r, int c)
    {
        assert(r >= 0 && c >= 0 && r < rows_ && c < cols_);
        return data_[r*cols_ + c];
    }
    const T& operator()(int r, int c) const
    {
        assert(r >= 0 && c >= 0 && r < rows_ && c < cols_);
        return data_[r*cols_ + c];
    }

    T* data() { return data_.data(); }
    const T* data() const { return data_.data(); }
    int size() const { return rows_ * cols_; }

    char* rawdata() { return reinterpret_cast<char*>(data_.data()); }
    const char* rawdata() const { return reinterpret_cast<const char*>(data_.data()); }
    int rawsize() const { return rows_ * cols_ * sizeof(T); }

    auto begin() { return data_.begin(); }
    auto begin() const { return data_.begin(); }
    auto end() { return data_.end(); }
    auto end() const { return data_.end(); }

	template <typename Iter>
	void assign(Iter first, Iter last) {
		data_.assign(first, last);
	}
};
