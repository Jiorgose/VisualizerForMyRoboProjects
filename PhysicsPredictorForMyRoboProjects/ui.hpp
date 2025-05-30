#pragma once
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

void uiNewFrame();

void uiUpdate(int& fps);

void uiRender();

void uiInit(GLFWwindow* window);

void uiDestroy();