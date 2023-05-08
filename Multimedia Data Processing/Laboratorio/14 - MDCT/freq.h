#pragma once
#include <unordered_map>

template<typename T>
class FrequencyCounter {
	std::unordered_map<T, uint32_t> occurencies_;
public:
	void operator()(const T &sym) {
		++occurencies_[sym];
	}

	auto operator[](T &sym) { return occurencies_[sym]; }

	auto begin() { return occurencies_.begin(); }
	auto begin() const { return occurencies_.begin(); }
	auto end() { return occurencies_.end(); }
	auto end() const { return occurencies_.end(); }

	double entropy() {
		double tot = 0;
		for (const auto &x : occurencies_) {
			tot += x.second;
		}
		double H = 0;
		for (const auto &x : occurencies_) {
			double px = x.second / tot;
			H += px * log2(px);
		}
		return -H;
	}
};