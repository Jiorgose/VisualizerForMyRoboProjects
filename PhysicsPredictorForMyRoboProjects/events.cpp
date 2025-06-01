#include "events.hpp"

static int x, y, width, height;


void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  AppState* state = static_cast<AppState*>(glfwGetWindowUserPointer(window));

  bool fullscreen = state->fullscreen;

  if (key == GLFW_KEY_F4 && action == GLFW_PRESS) {
    fullscreen = !fullscreen;
    
    if (fullscreen) {
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
    fullscreen = !fullscreen;
    if (!fullscreen) {
      glfwSetWindowMonitor(window, nullptr, x, y, width, height, 0);
    }
  }

  if (key == GLFW_KEY_W) {
    state->cameraPosition[2] += 0.1f;
  }
  if (key == GLFW_KEY_S) {
    state->cameraPosition[2] -= 0.1f;
  }
  if (key == GLFW_KEY_D) {
    state->cameraPosition[0] += 0.1f;
  }
  if (key == GLFW_KEY_A) {
    state->cameraPosition[0] -= 0.1f;
  }
  if (key == GLFW_KEY_SPACE) {
    state->cameraPosition[1] += 0.1f;
  }
  if (key == GLFW_KEY_LEFT_SHIFT) {
    state->cameraPosition[1] -= 0.1f;
  }
}
