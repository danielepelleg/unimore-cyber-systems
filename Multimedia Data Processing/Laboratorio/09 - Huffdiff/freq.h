#pragma once
#include <unordered_map>

template<typename symT, typename countT = uint32_t>
struct frequency_counter {
	std::unordered_map<symT, countT> occurrencies_;

	void operator()(const symT &sym) {
		++occurrencies_[sym];
	}

	auto operator[](symT &sym) {
		return occurrencies_[sym];
	}

	auto begin() { return occurrencies_.begin(); }
	auto begin() const { return occurrencies_.begin(); }
	auto end() { return occurrencies_.end(); }
	auto end() const { return occurrencies_.end(); }

	double entropy() {
		double tot = 0.0;
		for (const auto &x : occurrencies_) {
			tot += x.second;
		}

		double H = 0.0;
		for (const auto &x : occurrencies_) {
			double px = x.second / tot;
			H += px * log2(px);
		}
		return H;
	}
};

#pragma once
