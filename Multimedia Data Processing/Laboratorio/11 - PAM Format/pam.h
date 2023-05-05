#pragma once
#include <string>
#include "matrix.h"

bool write_pam(const std::string &filename, const Matrix<uint8_t> &img);

bool read_pam(const std::string &filename, Matrix<uint8_t> &img);