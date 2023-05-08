#pragma once

#include<vector>
#include<string>

#include"matrix.h"
#include "types.h"

double upsample(const Matrix<uint8_t>& img, int r, int c);
bool y4m_extract_color(const std::string& filename, std::vector<Matrix<vec3b>>& frames);