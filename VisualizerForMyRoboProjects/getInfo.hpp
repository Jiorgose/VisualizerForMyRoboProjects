#pragma once

#include <iostream>
#include <thread>
#include <string>
#include <glm.hpp>
#include <regex>
#include <sstream>
#include <GLFW/glfw3.h>
#include <serial/serial.h>

#include "common.hpp"

void initSerial(const std::string& port);
void closeSerial();
void updateSerial(GLFWwindow* window);
void scanForArduinoPorts(bool* autoSelect, int* selectedPortIndex, std::vector<std::string>* arduinoPorts);