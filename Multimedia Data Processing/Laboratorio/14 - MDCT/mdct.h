#pragma once

#define _USE_MATH_DEFINES
#include <math.h>

#include <vector>
#include <algorithm>

template<typename T>
auto quantize(const std::vector<T> &samples, int Q = 2600) {
	std::vector<int16_t> quantized(samples.size());
	std::transform(samples.begin(), samples.end(), quantized.begin(),
		[&Q](T sample) { 
			return static_cast<int16_t>(std::round(sample / Q)); 
		}
	);
	return quantized;
}

template<typename T>
auto dequantize(std::vector<int16_t> &quantized, double Q) {
	std::vector<T> dequantized(quantized.size());
	std::transform(quantized.begin(), quantized.end(), dequantized.begin(),
		[&Q](int16_t sample) {
			return static_cast<T>(sample * Q);
		}
	);
	return dequantized;
}

auto compute_error(std::vector<int16_t> &original_samples, std::vector<int16_t> &dequantized_samples) {
	std::vector<int16_t> error(original_samples.size());
	std::transform(begin(original_samples), end(original_samples), begin(dequantized_samples),
		begin(error), [](int16_t s, int16_t r) { return s - r; }
	);
	return error;
}

class MDCT {
	int N_;									// number of samples
	std::vector<double> wt;					// window's function
	std::vector<std::vector<double>> ct;	// cosine transform

public:
	MDCT(int N) : N_(N), wt(2 * N), ct(N, std::vector<double>(2 * N)) {
		for (int n = 0; n < N_; ++n) {
			wt[n] = sin(M_PI / (2 * n) * (n + 0.5));
		}

		for (int k = 0; k < N_; ++k) {
			for (int n = 0; n < N_ * 2; ++n) {
				ct[k][n] = cos(M_PI / N * (n + 0.5 + N / 2) * (k + 0.5));
			}
		}
	}

	void forward(const std::vector<int16_t>& x, std::vector<double>& X, int window_index) {
		for (int k = 0; k < N_; ++k) {
			double Xk = 0.0;
			auto& ctk = ct[k];
			for (int n = 0; n < 2 * N_; ++n) {
				int16_t xn = x[window_index * N_ + n];
				double wn = wt[n];
				double c = ctk[n];

				Xk += xn * wn * c;
			}
			X[window_index * N_ + k] = Xk;
		}
	}

	void backward(const std::vector<double>& X, std::vector<double>& y, int window_index){
		for (int n = 0; n < 2 * N_; ++n) {
			double yn = 0.0;
			for (int k = 0; k < N_; ++k) {
				double Xk = X[window_index*N_ + k];
				double c = ct[k][n];

				yn += Xk * c;
			}
			double wn = wt[n];
			y[n] = 2.0 / N_ * wn * yn;
		}
	}
};