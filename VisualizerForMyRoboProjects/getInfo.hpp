#pragma once

#include <iostream>
#include <thread>
#include <serial/serial.h>

void initSerial(const std::string& port);
void closeSerial();
void updateSerial();