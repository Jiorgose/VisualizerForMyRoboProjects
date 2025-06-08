#include "physics.hpp"

void updatePhysics(GLFWwindow* window) {
  AppState* state = static_cast<AppState*>(glfwGetWindowUserPointer(window));

  state->objectPosition = glm::vec3(sinf(glfwGetTime()), sinf(glfwGetTime()), sinf(glfwGetTime()));
}