#ifndef PGM_H
#define PGM_H

#include <string>
#include "mat.h"
#include "types.h"

bool save_pgm(const std::string& filename, const mat<uint8_t>& img, bool ascii = false);
bool load_pgm(const std::string& filename, mat<uint8_t>& img);

#endif // PGM_H