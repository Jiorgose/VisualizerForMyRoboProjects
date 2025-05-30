#include <cmath>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "events.hpp"
#include "ui.hpp"
#include "window.hpp"
#include "render.hpp"

static const double pi = 2 * acos(0.0);

double t = 0;
double dt = 0;
double frameTime = 0;
double updateTime = 0;

int fps = 60;
static const int width = 800;
static const int height = 600;

int main()
{
  GLFWwindow* window = createWindow(width, height, "lil render");
  glfwSetKeyCallback(window, keyCallback);

  uiInit(window);

  bool fullscreen = false;
  glfwSetWindowUserPointer(window, &fullscreen);

  while (!glfwWindowShouldClose(window)) {
    //time stuff :)
    t = glfwGetTime();
    dt = t - frameTime;
    updateTime = t;

    //Input
    glfwPollEvents();

    //fixed framerate
    if ((t - frameTime) >= (1.0 / (double)fps)) {
      uiNewFrame();
      uiUpdate(fps, dt);

      render(t, pi);
      
      uiRender();

      glfwSwapBuffers(window);

      frameTime = t;
    }
  }

  uiDestroy();

  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}