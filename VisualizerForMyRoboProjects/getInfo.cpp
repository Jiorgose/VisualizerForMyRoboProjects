#include "getInfo.hpp"

static std::unique_ptr<serial::Serial> mainSerial = nullptr;
static std::string outputString = "temp\n";
static std::string serialBuffer;

void initSerial(const std::string& port) {
  try {
    mainSerial = std::make_unique<serial::Serial>(port, 9600, serial::Timeout::simpleTimeout(3000));

    if (mainSerial->isOpen()) {
      std::cout << "Opened successfully on port " << port << std::endl;
    }
    else {
      std::cout << "Failed to open port " << port << std::endl;
    }

    mainSerial->flushOutput();
  }
  catch (const std::exception& e) {
    std::cerr << "Exception opening port: " << e.what() << std::endl;
  }
}

void closeSerial() {
  if (mainSerial && mainSerial->isOpen()) {
    mainSerial->close();
    mainSerial.reset();
    std::cout << "Serial port closed." << std::endl;
  }
}

std::string trim(const std::string& str) {
  const char* whitespace = " \t\n\r\f\v";
  size_t start = str.find_first_not_of(whitespace);
  if (start == std::string::npos)
    return "";

  size_t end = str.find_last_not_of(whitespace);
  return str.substr(start, end - start + 1);
}
 
void updateSerial() {
  if (!mainSerial) {
    std::cout << "Serial port not initialized!" << std::endl;
    return;
  }

  size_t bytesAvailable = mainSerial->available();
  if (bytesAvailable > 0) {
    std::string chunk = mainSerial->read(bytesAvailable);
    serialBuffer += chunk;

    size_t pos;
    while ((pos = serialBuffer.find('\n')) != std::string::npos) {
      std::string message = serialBuffer.substr(0, pos);
      serialBuffer.erase(0, pos + 1);

      message = trim(message);

      if (!message.empty()) {
        std::cout << "Received full message: " << message << std::endl;
      }
    }
  }
}