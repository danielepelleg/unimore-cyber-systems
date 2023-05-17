#ifndef IMAGE_OPERATIONS_H
#define IMAGE_OPERATIONS_H

#include <algorithm>
using namespace std;

#include "image.h"

template<typename T>
void flip(image<T>& img) {
	for (unsigned y = 0; y < img.height()/2; ++y) {
		for (unsigned x = 0; x < img.width(); ++x) {
			swap(img(x, y), img(x, img.height() - 1 - y));
		}
	}
}

template<typename T>
void paste(image<T>& dest, const image<T>& src, unsigned x0, unsigned y0) {
	for (unsigned y = 0; y < src.height(); ++y) {
		for (unsigned x = 0; x < src.width(); ++x) {
			unsigned X = x0 + x;
			unsigned Y = y0 + y;

			if (X < dest.width() && Y < dest.height())
				dest(X, Y) = src(x, y);
		}
	}
}

#endif // IMAGE_OPERATIONS_H