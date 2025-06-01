#pragma once

struct AppState {
  bool fullscreen = false;
  float renderWidth = 100.0;
  float renderHeight = 100.0;
  float color[3] = { 1.0f, 0.0f, 0.0f };
  float cameraPosition[3] = { 0.0f, 0.0f, -3.0f };
};