#pragma once
#include<string>
#include<vector>

#include "matrix.h"

bool y4m_extract_gray(const std::string& filename, std::vector<Matrix<uint8_t>>& frames);