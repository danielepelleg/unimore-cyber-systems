#ifndef UTILS_H
#define UTILS_H

#include <ios>

template<typename T>
std::ostream& raw_write(std::ostream& os, const T& val, size_t size = sizeof(T)) 
{
	return os.write(reinterpret_cast<const char*>(&val), size);
}

template <typename T>
std::istream& raw_read(std::istream& is, T& val, size_t size = sizeof(T))
{
	return is.read(reinterpret_cast<char*>(&val), size);
}
#endif // UTILS_H