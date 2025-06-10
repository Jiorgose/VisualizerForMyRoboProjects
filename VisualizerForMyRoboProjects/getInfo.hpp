#pragma once

#include <iostream>
#include <thread>
#include <string>
#include <serial/serial.h>

void initSerial(const std::string& port);
void closeSerial();
void updateSerial();
void scanForArduinoPorts(bool* autoSelect, int* selectedPortIndex, std::vector<std::string>* arduinoPorts);