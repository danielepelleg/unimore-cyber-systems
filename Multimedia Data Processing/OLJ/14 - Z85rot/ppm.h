#pragma once
#include <string>
#include <array>
#include "mat.h"

bool save_ppm(const std::string& filename, const mat<vec3b>& img, bool ascii = false);
bool load_ppm(const std::string& filename, mat<vec3b>& img);
