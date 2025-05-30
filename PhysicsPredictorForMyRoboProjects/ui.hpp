#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

void uiNewFrame();

void uiUpdate(int& fps, double dt, int width, int height, GLuint textureId);

void uiRender();

void uiInit(GLFWwindow* window);

void uiDestroy();