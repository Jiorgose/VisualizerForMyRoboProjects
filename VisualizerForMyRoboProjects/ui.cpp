#include "ui.hpp"

void uiNewFrame()
{
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
}

static bool autoSelect = false;
static int selectedPortIndex = -1;
static std::vector<std::string> arduinoPorts;

static const size_t max_history_size = 100;

static std::vector<float> pitch_history(max_history_size, 0.0f);
static std::vector<float> yaw_history(max_history_size, 0.0f);
static std::vector<float> roll_history(max_history_size, 0.0f);

static std::vector<float> X_history(max_history_size, 0.0f);
static std::vector<float> Y_history(max_history_size, 0.0f);
static std::vector<float> Z_history(max_history_size, 0.0f);

void updateRotationData(AppState* state) {
  glm::vec3 euler = glm::eulerAngles(state->objectRotation);
  float pitch = glm::degrees(euler.x);
  float yaw = glm::degrees(euler.y);
  float roll = glm::degrees(euler.z);

  pitch_history.push_back(pitch);
  yaw_history.push_back(yaw);
  roll_history.push_back(roll);

  if (pitch_history.size() > max_history_size) pitch_history.erase(pitch_history.begin());
  if (yaw_history.size() > max_history_size) yaw_history.erase(yaw_history.begin());
  if (roll_history.size() > max_history_size) roll_history.erase(roll_history.begin());
}

void updateAccelerationData(AppState* state) {
  glm::vec3 acc = glm::vec3(state->objectAcceleration);
  float X = acc.x * 9.81f;
  float Y = acc.y * 9.81f;
  float Z = acc.z * 9.81f;

  X_history.push_back(X);
  Y_history.push_back(Y);
  Z_history.push_back(Z);

  if (X_history.size() > max_history_size) X_history.erase(X_history.begin());
  if (Y_history.size() > max_history_size) Y_history.erase(Y_history.begin());
  if (Z_history.size() > max_history_size) Z_history.erase(Z_history.begin());
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

    ImGuiID mainId = dockspaceId;
    ImGuiID rightId = ImGui::DockBuilderSplitNode(mainId, ImGuiDir_Right, 0.3f, nullptr, &mainId);

    ImGuiID bottomId = ImGui::DockBuilderSplitNode(mainId, ImGuiDir_Down, 0.3f, nullptr, &mainId);
    ImGuiID midId = ImGui::DockBuilderSplitNode(mainId, ImGuiDir_Down, 0.28f, nullptr, &mainId);
    ImGuiID topId = mainId;

    ImGuiID rotationId = midId;
    ImGuiID accelerationId = ImGui::DockBuilderSplitNode(midId, ImGuiDir_Right, 0.5f, nullptr, &rotationId);

    ImGui::DockBuilderDockWindow("Scene", topId);
    ImGui::DockBuilderDockWindow("Rotation", rotationId);
    ImGui::DockBuilderDockWindow("Acceleration", accelerationId);
    ImGui::DockBuilderDockWindow("Settings", rightId);

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
  
  if (ImGui::CollapsingHeader("Program Information", ImGuiCond_Once)) {
    ImGui::SliderInt("Target FPS", &state->fps, 1, 120);
    ImGui::Separator();
    ImGui::Value("FPS: ", static_cast<float>(1.0 / state->dt));
  }

  if (ImGui::CollapsingHeader("Serial Console", ImGuiCond_Once)) {
    ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(0, 0, 0, 255));
    ImGui::BeginChild("Console", ImVec2(0, 150), true, ImGuiWindowFlags_HorizontalScrollbar);

    for (const auto& msg : state->messages) {
      ImGui::Text("%s", msg.c_str());
    }

    ImGui::EndChild();
    ImGui::PopStyleColor();
  }

  if (ImGui::CollapsingHeader("Select Arduino", ImGuiTreeNodeFlags_DefaultOpen)) {
    if (ImGui::Button("Refresh Arduino Ports")) {
      scanForArduinoPorts(&autoSelect, &selectedPortIndex, &arduinoPorts);
    }

    ImGui::Checkbox("Auto Select Arduino", &autoSelect);

    ImGui::Separator();

    if (arduinoPorts.empty()) {
      ImGui::Text("No Arduino devices found.");
    }
    else {
      for (int i = 0; i < (int)arduinoPorts.size(); ++i) {
        bool selected = (selectedPortIndex == i);
        if (ImGui::RadioButton(arduinoPorts[i].c_str(), selected)) {
          selectedPortIndex = i;
          closeSerial();
          initSerial(arduinoPorts[selectedPortIndex]);
        }
      }
    }
  }

  ImGui::End();

  updateRotationData(state);
  updateAccelerationData(state);

  ImGui::Begin("Rotation");

  availSize = ImGui::GetContentRegionAvail();

  if (ImPlot::BeginPlot("Rotation in degrees")) {
    ImPlot::SetupAxisLimits(ImAxis_X1, 0, max_history_size, ImGuiCond_Always);
    ImPlot::SetupAxisLimits(ImAxis_Y1, -180, 180, ImGuiCond_Always);

    int n = (int)pitch_history.size();
    if (n > 0) {
      std::vector<float> x_vals(n);
      std::iota(x_vals.begin(), x_vals.end(), 0);

      ImPlot::PlotLine("Pitch", x_vals.data(), pitch_history.data(), n);
      ImPlot::PlotLine("Yaw", x_vals.data(), yaw_history.data(), n);
      ImPlot::PlotLine("Roll", x_vals.data(), roll_history.data(), n);
    }
    ImPlot::EndPlot();
  }

  ImGui::End();

  ImGui::Begin("Acceleration");

  if (ImPlot::BeginPlot("Acceleration in m/s^2")) {
    ImPlot::SetupAxisLimits(ImAxis_X1, 0, max_history_size, ImGuiCond_Always);
    ImPlot::SetupAxisLimits(ImAxis_Y1, -15, 15, ImGuiCond_Always);

    int n = (int)X_history.size();
    if (n > 0) {
      std::vector<float> x_vals(n);
      std::iota(x_vals.begin(), x_vals.end(), 0);

      ImPlot::PlotLine("X", x_vals.data(), X_history.data(), n);
      ImPlot::PlotLine("Y", x_vals.data(), Y_history.data(), n);
      ImPlot::PlotLine("Z", x_vals.data(), Z_history.data(), n);
    }
    ImPlot::EndPlot();
  }

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
  ImPlot::CreateContext();
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
  ImPlot::DestroyContext();
}