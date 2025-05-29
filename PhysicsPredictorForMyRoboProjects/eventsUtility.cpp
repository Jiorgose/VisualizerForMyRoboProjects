#include "eventsUtility.hpp"
#include <GLFW/glfw3.h>
#include <iostream>

static int x, y, width, height;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  bool* fullscreen = static_cast<bool*>(glfwGetWindowUserPointer(window));

  if (key == GLFW_KEY_F4 && action == GLFW_PRESS) {
    *fullscreen = !*fullscreen;
    
    if (*fullscreen) {
      glfwGetWindowPos(window, &x, &y);
      glfwGetWindowSize(window, &width, &height);

      GLFWmonitor* monitor = glfwGetPrimaryMonitor();
      const GLFWvidmode* mode = glfwGetVideoMode(monitor);
      glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(), 0, 0, mode->width, mode->height, mode->refreshRate);
    }
    else {
      glfwSetWindowMonitor(window, nullptr, x, y, width, height, 0);
    }
  }
  
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    *fullscreen = !*fullscreen;
    if (!*fullscreen) {
      glfwSetWindowMonitor(window, nullptr, x, y, width, height, 0);
    }
  }
}
