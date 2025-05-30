#include <cmath>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "events.hpp"
#include "ui.hpp"
#include "window.hpp"
#include "render.hpp"

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

const char* vertexShaderCode = R"*(
#version 330

layout (location = 0) in vec3 pos;

void main()
{
	gl_Position = vec4(0.9*pos.x, 0.9*pos.y, 0.5*pos.z, 1.0);
}
)*";

const char* fragmentShaderCode = R"*(
#version 330

out vec4 color;

void main()
{
	color = vec4(0.0, 1.0, 0.0, 1.0);
}
)*";

int main()
{
  GLFWwindow* window = createWindow(width, height, "lil render");
  glfwSetKeyCallback(window, keyCallback);

  uiInit(window);

  bool fullscreen = false;
  glfwSetWindowUserPointer(window, &fullscreen);

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
      render(t, width, height, shader, textureId, RBO, FBO, VAO);
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