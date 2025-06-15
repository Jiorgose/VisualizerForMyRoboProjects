#pragma once

#include <glm.hpp>
#include <gtc/quaternion.hpp>
#include <vector>

struct AppState {
  double t = 0;
  double dt = 0;
  double frameTime = 0;
  double updateTime = 0;
  int fps = 30;

  bool isInScene = false;
  double velocityX = 0.0;
  double velocityY = 0.0;

  bool fullscreen = false;
  float renderWidth = 100.0f;
  float renderHeight = 100.0f;
  glm::vec2 mousePosition = glm::vec2(0.0f);

  glm::vec3 objectPosition = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::quat objectRotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
};