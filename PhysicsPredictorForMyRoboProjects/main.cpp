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

double PI = 3.14159265359;

AppState state;

static const int width = 800;
static const int height = 600;

GLuint VAO;
GLuint VBO;
GLuint FBO;
GLuint RBO;
GLuint textureId;

GLuint shader;

const double clamp(const double& value, const double& low, const double& high)
{
  return (value < low) ? low : (high < value) ? high : value;
}

int main(int argc, char* argv[])
{
  GLFWwindow* window = createWindow(width, height, "lil render");
  glfwSetKeyCallback(window, keyCallback);
  glfwSetCursorPosCallback(window, cursorPositionCallback);

  uiInit(window);

  glfwSetWindowUserPointer(window, &state);

  //Set window Icon
  int iconWidth, iconHeight;
  int iconChannels;

  unsigned char* icon = loadIcon("../Icon.png", &iconWidth, &iconHeight, &iconChannels, 4);
  std::cout << icon << std::endl;
  GLFWimage icons[1];
  icons[0].width = iconWidth;
  icons[0].height = iconHeight;
  icons[0].pixels = icon;
  glfwSetWindowIcon(window, 1, icons);

  std::string vertexShaderCodeStr = loadShaderSource("../Shaders/vertex.glsl");
  const char* vertexShaderCode = vertexShaderCodeStr.c_str();

  std::string fragmentShaderCodeStr = loadShaderSource("../Shaders/fragment.glsl");
  const char* fragmentShaderCode = fragmentShaderCodeStr.c_str();

  create_triangle(VAO, VBO);
  create_shaders(shader, vertexShaderCode, fragmentShaderCode);
  create_framebuffer(int(state.renderWidth), int(state.renderHeight), textureId, FBO, RBO, &state);

  while (!glfwWindowShouldClose(window)) {
    //time stuff :)
    state.t = glfwGetTime();
    state.dt = state.t - state.frameTime;
    state.updateTime = state.t;

    //Input
    glfwPollEvents();

    //fixed framerate
    if ((state.t - state.frameTime) >= (1.0 / (double)state.fps)) {
      state.velocityX *= 0.9;
      state.velocityY *= 0.9;

      state.mousePosition[0] += state.velocityX;
      state.mousePosition[1] += state.velocityY;
      double normalizedY = state.mousePosition[1] / state.renderHeight;
      normalizedY = clamp(normalizedY, 0.1, 0.8);
      state.mousePosition[1] = normalizedY * state.renderHeight;


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