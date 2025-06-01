#include <cmath>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "common.hpp"
#include "events.hpp"
#include "ui.hpp"
#include "window.hpp"
#include "render.hpp"
#include "load.hpp"

AppState state;

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

  glfwSetWindowUserPointer(window, &state);

  std::string vertexShaderCodeStr = loadShaderSource("../Shaders/vertex.glsl");
  const char* vertexShaderCode = vertexShaderCodeStr.c_str();

  std::string fragmentShaderCodeStr = loadShaderSource("../Shaders/fragment.glsl");
  const char* fragmentShaderCode = fragmentShaderCodeStr.c_str();

  create_triangle(VAO, VBO);
  create_shaders(shader, vertexShaderCode, fragmentShaderCode);
  create_framebuffer(state.renderWidth, state.renderHeight, textureId, FBO, RBO, &state);

  while (!glfwWindowShouldClose(window)) {
    //time stuff :)
    state.t = glfwGetTime();
    state.dt = state.t - state.frameTime;
    state.updateTime = state.t;

    //Input
    glfwPollEvents();

    //fixed framerate
    if ((state.t - state.frameTime) >= (1.0 / (double)state.fps)) {
      uiNewFrame();
      render(shader, textureId, RBO, FBO, VAO, window);
      uiUpdate(textureId, shader, window);

      uiRender();

      glfwSwapBuffers(window);

      state.frameTime = state.t;
    }
  }

  uiDestroy();

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteProgram(shader);

  glDeleteFramebuffers(1, &FBO);
  glDeleteTextures(1, &textureId);
  glDeleteRenderbuffers(1, &RBO);

  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}