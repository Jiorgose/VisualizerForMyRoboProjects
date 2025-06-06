#include "mouseMovement.hpp"

const double clamp(const double& value, const double& low, const double& high)
{
  return (value < low) ? low : (high < value) ? high : value;
}

static bool wasMouseDown = false;
static double mousePos[2];

void moveMouse(GLFWwindow *window)
{
  AppState* state = static_cast<AppState*>(glfwGetWindowUserPointer(window));

  bool isMouseDown = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;

  double xpos, ypos;
  glfwGetCursorPos(window, &xpos, &ypos);

  if (isMouseDown && !wasMouseDown) {
    mousePos[0] = xpos;
    mousePos[1] = ypos;
  }

  if (isMouseDown && state->isInScene) {
    double dx = xpos - mousePos[0];
    double dy = ypos - mousePos[1];

    state->velocityX = dx / (4096.0 / state->renderWidth);
    state->velocityY = dy / (2160.0 / state->renderHeight);

    mousePos[0] = xpos;
    mousePos[1] = ypos;
  }

  wasMouseDown = isMouseDown;

  state->velocityX *= 0.9;
  state->velocityY *= 0.9;

  state->mousePosition[0] += state->velocityX;
  state->mousePosition[1] += state->velocityY;
  double normalizedY = state->mousePosition[1] / state->renderHeight;
  normalizedY = clamp(normalizedY, 0.1, 0.5);
  state->mousePosition[1] = normalizedY * state->renderHeight;
}