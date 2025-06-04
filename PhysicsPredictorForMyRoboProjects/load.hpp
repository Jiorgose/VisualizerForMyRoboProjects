#pragma once

#include <fstream>
#include <sstream>
#include <string>

std::string loadShaderSource(const std::string& filePath);

unsigned char* loadIcon(char const* filename, int* x, int* y, int* comp, int req_comp);