#ifndef PGM_H
#define PGM_H

#include <string>

#include "types.h"
#include "image.h"

bool writeP2(const std::string& sFileName, const image<mybyte>& img);
bool writeP5(const std::string& sFileName, const image<mybyte>& img);

bool readP2(const std::string& sFileName, image<mybyte>& img);
bool readP5(const std::string& sFileName, image<mybyte>& img);

#endif // PGM_H