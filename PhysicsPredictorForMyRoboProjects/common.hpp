#pragma once

#include <imgui.h>

struct AppState {
  bool fullscreen = false;
  float color[3] = { 1.0f, 0.0f, 0.0f };
  float cameraPosition[3] = { 0.0f, 0.0f, -3.0f };
};