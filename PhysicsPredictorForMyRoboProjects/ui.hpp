#pragma once
#include "common.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <gtc/type_ptr.hpp>

void uiNewFrame();

void uiUpdate(GLuint textureId, GLuint fragmentShader, GLFWwindow* window);

void uiRender();

void uiInit(GLFWwindow* window);

void uiDestroy();