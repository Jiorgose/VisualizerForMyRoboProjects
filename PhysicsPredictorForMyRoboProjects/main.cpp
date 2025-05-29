#include <glad/glad.h>
#include <glad/glad.c>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <iostream>

#include "eventsUtility.hpp"

static const int width = 800;
static const int height = 600;

static const double pi = 2 * acos(0.0);

int main()
{
  glfwInit();

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(width, height, "Physics Predictor", NULL, NULL);

  glfwMakeContextCurrent(window);
  gladLoadGL();
  glfwSwapInterval(1);
  glfwSetKeyCallback(window, keyCallback);

  bool fullscreen = false;
  glfwSetWindowUserPointer(window, &fullscreen);

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
    
    double t = glfwGetTime();

    const float red = (float)(0.5 + 0.5 * sin(t));
    const float green = (float)(0.5 + 0.5 * sin(t + pi * 2 / 3));
    const float blue = (float)(0.5 + 0.5 * sin(t + pi * 4 / 3));

    glClearColor(red, green, blue, 1.0); 

    glClear(GL_COLOR_BUFFER_BIT);

    glfwSwapBuffers(window);
  }

  glfwDestroyWindow(window);

  glfwTerminate();
  return 0;
}
