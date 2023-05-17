#ifndef PPM_H
#define PPM_H

#include <string>

#include "types.h"
#include "image.h"

bool writeP3(const std::string& sFileName, const image<vec3b>& img);
bool writeP6(const std::string& sFileName, const image<vec3b>& img);

bool readP3(const std::string& sFileName, image<vec3b>& img);
bool readP6(const std::string& sFileName, image<vec3b>& img);

#endif // PPM_H