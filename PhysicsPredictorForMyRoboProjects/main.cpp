#include <iostream>
#include <GLFW/glfw3.h>

#include "eventsUtility.hpp"

static const int width = 800;
static const int height = 600;

int main()
{
  glfwInit();

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(width, height, "Physics Predictor", NULL, NULL);

  glfwMakeContextCurrent(window);
  glfwSetKeyCallback(window, keyCallback);

  bool fullscreen = false;
  glfwSetWindowUserPointer(window, &fullscreen);

  while (!glfwWindowShouldClose(window)) {
    glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glfwSwapBuffers(window);

    glfwPollEvents();
  }

  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}
