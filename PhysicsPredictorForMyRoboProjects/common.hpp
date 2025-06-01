#pragma once

struct AppState {
  double t = 0;
  double dt = 0;
  double frameTime = 0;
  double updateTime = 0;
  int fps = 60;

  bool fullscreen = false;
  float renderWidth = 100.0f;
  float renderHeight = 100.0f;
  float color[3] = { 1.0f, 0.0f, 0.0f };
  float cameraPosition[3] = { 0.0f, 0.0f, -3.0f };
};