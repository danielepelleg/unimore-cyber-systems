#pragma once

#include <string>

#include "types.h"
#include "mat.h"

bool load_pcx(const std::string& filename, mat<vec3b>& img);

bool load_pcx(const std::string& filename, mat<uint8_t>& img);

bool read_pcx_line(std::istream& is, mat<uint8_t>& img, int row, uint32_t total_bytes);

bool read_pcx_line(std::istream& is, mat<vec3b>& img, int row, uint32_t total_bytes, uint32_t skip_bytes);