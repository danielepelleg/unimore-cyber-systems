#include "ppm.h"

#include <istream>

std::istream& operator>>(std::istream& is, vec3b& v)
{
    return is >> v[0] >> v[1] >> v[2];
}