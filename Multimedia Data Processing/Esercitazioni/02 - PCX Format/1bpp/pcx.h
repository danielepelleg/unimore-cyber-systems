#pragma once

#include <string>
#include "mat.h"

bool load_pcx(const std::string& filename, mat<uint8_t>& img);

bool read_pcx_line(std::istream& is, mat<uint8_t>& img, int row, uint32_t total_bytes);