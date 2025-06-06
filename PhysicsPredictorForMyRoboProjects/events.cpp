#include "events.hpp"

static int x, y, width, height;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  AppState* state = static_cast<AppState*>(glfwGetWindowUserPointer(window));

  if ((action == GLFW_PRESS)) {
    if (key == GLFW_KEY_F4) {
      state->fullscreen = !state->fullscreen;

      if (state->fullscreen) {
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

    if (key == GLFW_KEY_ESCAPE) {
      state->fullscreen = !state->fullscreen;
      if (!state->fullscreen) {
        glfwSetWindowMonitor(window, nullptr, x, y, width, height, 0);
      }
    }
  }
}