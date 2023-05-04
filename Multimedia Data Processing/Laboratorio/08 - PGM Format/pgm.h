#pragma once
#include <string>
#include "matrix.h"

enum pgm_mode { plain = 2, binary = 5 };

// must be a 8 bit image beacause PGM
// only support grey images
bool write(const std::string &filename, const matrix<uint8_t> &img, pgm_mode mode);