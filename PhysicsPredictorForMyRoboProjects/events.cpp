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

static double mousePos[2];

void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
{
  AppState* state = static_cast<AppState*>(glfwGetWindowUserPointer(window));

  if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE || !state->isInScene) {
    mousePos[0] = xpos;
    mousePos[1] = ypos;
    return;
  }

  state->velocityX = (xpos - mousePos[0]) * 10.0;
  state->velocityY = (ypos - mousePos[1]) * 10.0;

  mousePos[0] = xpos;
  mousePos[1] = ypos;
}