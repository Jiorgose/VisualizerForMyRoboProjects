#include "ui.hpp"

void uiNewFrame()
{
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
}

void uiUpdate(GLuint textureId, GLuint fragmentShader, GLFWwindow* window)
{
  AppState* state = static_cast<AppState*>(glfwGetWindowUserPointer(window));

  static ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_PassthruCentralNode;

  ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDocking;

  ImGuiViewport* viewport = ImGui::GetMainViewport();
  ImGui::SetNextWindowPos(viewport->Pos);
  ImGui::SetNextWindowSize(viewport->Size);
  ImGui::SetNextWindowViewport(viewport->ID);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
  windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
  windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
  windowFlags |= ImGuiWindowFlags_NoBackground;

  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
  ImGui::Begin("DockSpace", nullptr, windowFlags);
  ImGui::PopStyleVar();
  ImGui::PopStyleVar(2);

  ImGuiID dockspaceId = ImGui::GetID("MyDockSpace");
  ImGui::DockSpace(dockspaceId, ImVec2(0.0f, 0.0f), dockspaceFlags);

  static auto firstTime = true;
  if (firstTime) {
    firstTime = false;

    ImGui::DockBuilderRemoveNode(dockspaceId);
    ImGui::DockBuilderAddNode(dockspaceId, dockspaceFlags | ImGuiDockNodeFlags_DockSpace);
    ImGui::DockBuilderSetNodeSize(dockspaceId, viewport->Size);
    auto rightId = ImGui::DockBuilderSplitNode(dockspaceId, ImGuiDir_Right, 0.3f, nullptr, &dockspaceId);

    ImGui::DockBuilderDockWindow("Scene", dockspaceId);
    ImGui::DockBuilderDockWindow("Settings", rightId);
    ImGui::DockBuilderDockWindow("Info", rightId);

    ImGui::DockBuilderFinish(dockspaceId);
    ImGui::DockBuilderFinish(dockspaceId);
  }
  ImGui::End();

  ImGui::Begin("Scene");
  ImVec2 availSize = ImGui::GetContentRegionAvail();
  
  state->renderWidth = availSize.x;
  state->renderHeight = availSize.y;

  ImGui::Image((ImTextureID)(intptr_t)textureId, availSize, ImVec2(0, 1), ImVec2(1, 0));

  if (ImGui::IsWindowFocused()) {
    state->isInScene = true;
  }
  else {
    state->isInScene = false;
  }

  ImGui::End();

  ImGui::Begin("Settings");
  ImGui::DragFloat3("object position: ", state->objectPosition, 0.01f, -3.5f, 3.5f);
  ImGui::DragFloat3("object rotation: ", state->objectRotation, 0.1f, 0.0f, 360.0f);
  ImGui::End();

  ImGui::Begin("Info");
  ImGui::Text("I am currently running at %d fps", static_cast<int>(round(1.0 / state->dt)));
  ImGui::End();
}

void uiRender()
{
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void uiInit(GLFWwindow* window)
{
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  (void)io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

  ImGui::StyleColorsDark();

  ImGuiStyle& style = ImGui::GetStyle();

  style.TabRounding = 0.0f;
  style.WindowRounding = 0.0f;
  style.TabBorderSize = 0.0f;
  style.WindowBorderSize = 0.0f;
  style.FrameBorderSize = 0.0f;

  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 460");
}

void uiDestroy()
{
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}