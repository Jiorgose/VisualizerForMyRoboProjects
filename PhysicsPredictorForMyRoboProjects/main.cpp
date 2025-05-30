#include <cmath>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "events.hpp"
#include "ui.hpp"
#include "window.hpp"
#include "render.hpp"
#include "load.hpp"

double t = 0;
double dt = 0;
double frameTime = 0;
double updateTime = 0;

int fps = 60;
static const int width = 800;
static const int height = 600;

GLuint VAO;
GLuint VBO;
GLuint FBO;
GLuint RBO;
GLuint textureId;

GLuint shader;

int main(int argc, char* argv[])
{
  GLFWwindow* window = createWindow(width, height, "lil render");
  glfwSetKeyCallback(window, keyCallback);

  uiInit(window);

  bool fullscreen = false;
  glfwSetWindowUserPointer(window, &fullscreen);

  std::string vertexShaderCodeStr = loadShaderSource("../Shaders/vertex.glsl");
  const char* vertexShaderCode = vertexShaderCodeStr.c_str();

  std::string fragmentShaderCodeStr = loadShaderSource("../Shaders/fragment.glsl");
  const char* fragmentShaderCode = fragmentShaderCodeStr.c_str();

  create_triangle(VAO, VBO);
  create_shaders(shader, vertexShaderCode, fragmentShaderCode);
  create_framebuffer(width, height, textureId, FBO, RBO);

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
      render(t, width * 2, height * 2, shader, textureId, RBO, FBO, VAO);
      uiUpdate(fps, dt, textureId);
      
      uiRender();

      glfwSwapBuffers(window);

      frameTime = t;
    }
  }

  uiDestroy();

  glDeleteFramebuffers(1, &FBO);
  glDeleteTextures(1, &textureId);
  glDeleteRenderbuffers(1, &RBO);

  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}