#ifndef TYPES_H
#define TYPES_H

typedef unsigned char mybyte;
typedef unsigned int uint;
typedef unsigned short ushort;

template <typename _T, int _N>
struct vec {
	_T _data[_N];

	vec() {}
	vec (const _T& el0) {
		_data[0] = el0;
	}
	vec (const _T& el0, const _T& el1) {
		_data[0] = el0;
		_data[1] = el1;
	}
	vec (const _T& el0, const _T& el1, const _T& el2) {
		_data[0] = el0;
		_data[1] = el1;
		_data[2] = el2;
	}

	_T& operator[] (unsigned i) { return _data[i]; }
	const _T& operator[] (unsigned i) const { return _data[i]; }

	bool operator== (const vec& other) const {
		for (int i=0;i<_N;++i)
			if (_data[i]!=other._data[i])
				return false;
		return true;
	}
};

typedef vec<mybyte,3> vec3b;

#endif /* TYPES_H */