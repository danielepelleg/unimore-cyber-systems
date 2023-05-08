#include <iostream>
#include <cstdlib>
#include <string>
#include <algorithm>
#include <chrono>

#include "raw.h"
#include "freq.h"
#include "mdct.h"

int error(std::string msg) {
	std::cout << msg;
	return EXIT_FAILURE;
}

int main(int argc, char * argv[]) {
	{
		using clock = std::chrono::steady_clock;
		std::cout << "Start...\n";
		auto start_time = clock::now();

		std::cout << "Reading... ";
		auto samples = read_samples("test.raw");
		std::cout << "done.\n";
		std::cout << "Number of samples: " << samples.size() << "\n";

		std::cout << "Computing frequencies... ";
		auto f = for_each(begin(samples), end(samples), FrequencyCounter<int16_t>());
		std::cout << "done.\n";
		std::cout << "f.entropy(): " << f.entropy() << "\n";

		if (true) {
			// Quantize
			int Q = 2600;
			std::cout << "Quantizing... ";
			auto quantized = quantize(samples, Q);
			std::cout << "done.\n";

			std::cout << "Computing frequencies... ";
			auto fq = for_each(begin(quantized), end(quantized), FrequencyCounter<int16_t>());
			std::cout << "done.\n";
			std::cout << "fq.entropy(): " << fq.entropy() << "\n";

			// Reconstruct
			std::cout << "Quantizing... ";
			auto reconstructed = dequantize<int16_t>(quantized, Q);
			std::cout << "done.\n";
			std::cout << "Saving reconstructed signal... ";
			write_samples("reconstructed.raw", reconstructed);
			std::cout << "done.\n";

			// Error
			std::cout << "Computing error... ";
			auto error = compute_error(samples, reconstructed);
			std::cout << "done.\n";
			std::cout << "Saving error signal... ";
			write_samples("error.raw", error);
			std::cout << "done.\n";
		}

		if (true) {
			// MDCT
			std::cout << "Copying and padding samples... ";
			int N = 1024;
			int W = 2 * N; // Window size
			int num_win = (samples.size() + N - 1) / N;
			std::vector<int16_t> padded((num_win + 2)*N);
			copy(begin(samples), end(samples), next(begin(padded), N));
			std::cout << "done.\n";

			std::cout << "Precomputing sines and cosines... ";
			MDCT mdct(N);
			std::cout << "done.\n";

			std::cout << "Computing MDCT... ";
			std::vector<double> trans((num_win + 1)*N);
			for (int i = 0; i < num_win + 1; ++i) {
				mdct.forward(padded, trans, i);
			}
			std::cout << "done.\n";

			// Quantize
			std::cout << "Quantizing... ";
			double Q = 10000.;
			auto transq = quantize(trans, Q);
			std::cout << "done.\n";
			std::cout << "Computing frequencies... ";
			auto ftq = for_each(begin(transq), end(transq), FrequencyCounter<int16_t>());
			std::cout << "done.\n";
			std::cout << "ftq.entropy(): " << ftq.entropy() << "\n";

			// Reconstruct
			std::cout << "Dequantizing... ";
			auto transdq = dequantize<double>(transq, Q);
			std::cout << "done.\n";

			// IMDCT
			std::cout << "Computing IMDCT... ";
			std::vector<int16_t> transr(num_win*N);
			std::vector<double> prev_inv(2 * N);
			std::vector<double> curr_inv(2 * N);

			mdct.backward(transdq, prev_inv, 0);
			for (int i = 1; i < num_win + 1; ++i) {
				// Inverse MDCT
				mdct.backward(transdq, curr_inv, i);

				// Add 2nd half of prev to 1st half of curr
				std::transform(
					next(begin(prev_inv), N), end(prev_inv), // First operand From, To
					begin(curr_inv),                         // Second operand From (To is deduced from First)
					next(begin(transr), N * (i - 1)),        // Output From (To is deduced from First)
					[](double a, double b) { return static_cast<int16_t>(a + b); } // Predicate
				);

				// prev <- curr
				prev_inv = curr_inv;
			}
			std::cout << "done.\n";

			std::cout << "Saving reconstructed signal... ";
			write_samples("transr.raw", transr);
			std::cout << "done.\n";

			// Error
			std::cout << "Computing error... ";
			auto error = compute_error(samples, transr);
			std::cout << "done.\n";
			std::cout << "Saving error signal... ";
			write_samples("error_mdct.raw", error);
			std::cout << "done.\n";
		}

		auto stop_time = clock::now();
		std::cout << "Stop.\n";
		auto diff = stop_time - start_time;
		std::cout << "Elapsed: " << diff.count() / 1000000000.0 << " s\n";
	}
	_CrtDumpMemoryLeaks();
	return EXIT_SUCCESS;
}