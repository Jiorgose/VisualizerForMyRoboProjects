#include <glad/glad.c>
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <iostream>

#include "events.hpp"

static const double pi = 2 * acos(0.0);

double t = 0;
double dT = 0;
double frameTime = 0;
double updateTime = 0;

int fps = 60.0;
static const int width = 800;
static const int height = 600;

int main()
{
  glfwInit();

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(width, height, "Physics Predictor", NULL, NULL);

  glfwMakeContextCurrent(window);
  gladLoadGL();
  glfwSwapInterval(0);
  glfwSetKeyCallback(window, keyCallback);

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  (void)io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

  ImGui::StyleColorsDark();

  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 460");

  bool fullscreen = false;
  glfwSetWindowUserPointer(window, &fullscreen);

  while (!glfwWindowShouldClose(window)) {
    t = glfwGetTime();
    dT = t - updateTime;
    updateTime = t;
    glfwPollEvents();

    if ((t - frameTime) >= (1.0 / (double)fps)) {
      ImGui_ImplOpenGL3_NewFrame();
      ImGui_ImplGlfw_NewFrame();
      ImGui::NewFrame();

      ImGui::Begin("test");
      ImGui::Text("Hoi :)");
      ImGui::SliderInt("fps", &fps, 1, 120, nullptr, 0);
      ImGui::End();

      const float red = (float)(0.5 + 0.5 * sin(t));
      const float green = (float)(0.5 + 0.5 * sin(t + pi * 2 / 3));
      const float blue = (float)(0.5 + 0.5 * sin(t + pi * 4 / 3));

      glClearColor(red, green, blue, 1.0);

      glClear(GL_COLOR_BUFFER_BIT);
      ImGui::Render();
      ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

      glfwSwapBuffers(window);

      frameTime = t;
    }
  }

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}