#include "getInfo.hpp"

static serial::Serial mainSerial("COM5", 9600, serial::Timeout::simpleTimeout(3000));
static std::string outputString = "temp\n";

void initSerial() {
  if (mainSerial.isOpen()) {
    std::cout << "Opened successfuly" << std::endl;
  }
  else {
    std::cout << "your fault" << std::endl;
  }

  mainSerial.flushOutput();
}

void updateSerial() {
  size_t bytesWritten = mainSerial.write(outputString);

  std::cout << "Bytes Sent: " << bytesWritten << std::endl;
  std::this_thread::sleep_for(std::chrono::milliseconds(2000));
}