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

struct SensorData {
  glm::quat quat;
  glm::vec3 accel;
};

SensorData parseQuatAndAccel(const std::string& str) {
  SensorData data;
  size_t sep = str.find('|');

  if (sep != std::string::npos) {
    std::string quatStr = str.substr(0, sep);
    std::string accelStr = str.substr(sep + 1);

    std::regex quatPattern(R"((-?[\d\.eE+-]+),(-?[\d\.eE+-]+),(-?[\d\.eE+-]+),(-?[\d\.eE+-]+))");
    std::regex accelPattern(R"((-?[\d\.eE+-]+),(-?[\d\.eE+-]+),(-?[\d\.eE+-]+))");

    std::smatch quatMatch, accelMatch;

    if (std::regex_search(quatStr, quatMatch, quatPattern)) {
      float w = std::stof(quatMatch[1]);
      float x = std::stof(quatMatch[2]);
      float y = std::stof(quatMatch[3]);
      float z = std::stof(quatMatch[4]);
      data.quat = glm::quat(w, x, z, y);
    }

    if (std::regex_search(accelStr, accelMatch, accelPattern)) {
      data.accel.x = std::stof(accelMatch[1]);
      data.accel.y = std::stof(accelMatch[2]);
      data.accel.z = std::stof(accelMatch[3]);
    }
  }

  return data;
}
 
void updateSerial(GLFWwindow* window) {
  if (!mainSerial) {
    //std::cout << "Serial port not initialized!" << std::endl;
    return;
  }

  AppState* state = static_cast<AppState*>(glfwGetWindowUserPointer(window));

  try {
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
          SensorData sensData = parseQuatAndAccel(message);
          state->objectRotation = sensData.quat;
          state->objectAcceleration = sensData.accel;
        }
      }
    }
  }
  catch (const std::exception& e) {
    std::cerr << "Serial read error: " << e.what() << std::endl;
    closeSerial();
  }
}

void scanForArduinoPorts(bool* autoSelect, int* selectedPortIndex, std::vector<std::string>* arduinoPorts) {
  arduinoPorts->clear();

  auto ports = serial::list_ports();

  for (const auto& port : ports) {
    std::string desc = port.description;
    std::string hwid = port.hardware_id;

    bool isArduino = false;

    if (desc.find("Arduino") != std::string::npos) {
      isArduino = true;
    }
    else if (hwid.find("VID:PID=2341:") != std::string::npos) {
      isArduino = true;
    }
    else if (desc.find("USB Serial") != std::string::npos || hwid.find("USB") != std::string::npos) {
      isArduino = true;
    }

    if (isArduino) {
      arduinoPorts->push_back(port.port);
    }
  }

  if (autoSelect && !arduinoPorts->empty()) {
    *selectedPortIndex = 0;
    closeSerial();
    initSerial((*arduinoPorts)[*selectedPortIndex]);
  }
  else {
    *selectedPortIndex = -1;
  }
}