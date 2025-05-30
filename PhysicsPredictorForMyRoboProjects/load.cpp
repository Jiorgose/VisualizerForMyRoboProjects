#include "load.hpp"

std::string loadShaderSource(const std::string& filePath) {
  std::ifstream file(filePath);
  std::stringstream buffer;
  buffer << file.rdbuf();
  return buffer.str();
}
