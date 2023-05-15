#ifndef PPM_H
#define PPM_H

#include <string>
#include <array>
#include "mat.h"
#include "types.h"

bool save_ppm(const std::string& filename, const mat<vec3b>& img, bool ascii = false);
bool load_ppm(const std::string& filename, mat<vec3b>& img);

#endif // PPM_H