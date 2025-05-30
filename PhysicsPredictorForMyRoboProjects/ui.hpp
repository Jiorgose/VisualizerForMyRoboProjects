#pragma once
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

void uiNewFrame();

void uiUpdate(int& fps, double dt);

void uiRender();

void uiInit(GLFWwindow* window);

void uiDestroy();