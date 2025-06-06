#pragma once

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
  float mousePosition[2] = { 0.0f, 0.0f };

  float objectPosition[3] = { 0.0f, 0.0f, 0.0f };
  float objectRotation[3] = { 0.0f, 0.0f, 0.0f };
};