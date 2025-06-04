#include "load.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

std::string loadShaderSource(const std::string& filePath) {
  std::ifstream file(filePath);
  std::stringstream buffer;
  buffer << file.rdbuf();
  return buffer.str();
}

unsigned char* loadIcon(char const* filename, int* x, int* y, int* comp, int req_comp) {
  return stbi_load(filename, x, y, comp, req_comp);
}