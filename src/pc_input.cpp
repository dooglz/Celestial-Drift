#include "pc_input.h"
#include "CommandParser.h"
#include "genericInput.h"
#include "menu.h"
#include "pc_video.h"
#include <GLFW/glfw3.h>

#include <iostream>

std::vector<actual_Input> actual_Input_builtins = {
    {"Q", GLFW_KEY_Q, {}},         {"W", GLFW_KEY_W, {}},         {"E", GLFW_KEY_E, {}},
    {"R", GLFW_KEY_R, {}},         {"T", GLFW_KEY_T, {}},         {"Y", GLFW_KEY_Y, {}},
    {"U", GLFW_KEY_U, {}},         {"I", GLFW_KEY_I, {}},         {"O", GLFW_KEY_O, {}},
    {"P", GLFW_KEY_P, {}},         {"A", GLFW_KEY_A, {}},         {"S", GLFW_KEY_S, {}},
    {"D", GLFW_KEY_D, {}},         {"F", GLFW_KEY_F, {}},         {"G", GLFW_KEY_G, {}},
    {"H", GLFW_KEY_H, {}},         {"J", GLFW_KEY_J, {}},         {"K", GLFW_KEY_K, {}},
    {"L", GLFW_KEY_L, {}},         {"Z", GLFW_KEY_Z, {}},         {"X", GLFW_KEY_X, {}},
    {"C", GLFW_KEY_C, {}},         {"V", GLFW_KEY_V, {}},         {"B", GLFW_KEY_B, {}},
    {"N", GLFW_KEY_N, {}},         {"M", GLFW_KEY_M, {}},         {"0", GLFW_KEY_0, {}},
    {"1", GLFW_KEY_1, {}},         {"2", GLFW_KEY_2, {}},         {"3", GLFW_KEY_3, {}},
    {"4", GLFW_KEY_4, {}},         {"5", GLFW_KEY_5, {}},         {"6", GLFW_KEY_6, {}},
    {"7", GLFW_KEY_7, {}},         {"8", GLFW_KEY_8, {}},         {"9", GLFW_KEY_9, {}},
    {"NP1", GLFW_KEY_KP_1, {}},    {"NP2", GLFW_KEY_KP_2, {}},    {"NP3", GLFW_KEY_KP_3, {}},
    {"NP4", GLFW_KEY_KP_4, {}},    {"NP5", GLFW_KEY_KP_5, {}},    {"NP6", GLFW_KEY_KP_6, {}},
    {"NP7", GLFW_KEY_KP_7, {}},    {"NP8", GLFW_KEY_KP_8, {}},    {"NP9", GLFW_KEY_KP_9, {}},
    {"-", GLFW_KEY_MINUS, {}},     {"=", GLFW_KEY_EQUAL, {}},     {"ESC", GLFW_KEY_ESCAPE, {}},
    {"ENTER", GLFW_KEY_ENTER, {}}, {"SPACE", GLFW_KEY_SPACE, {}}, {"UP", GLFW_KEY_UP, {}},
    {"DOWN", GLFW_KEY_DOWN, {}},   {"LEFT", GLFW_KEY_LEFT, {}},   {"RIGHT", GLFW_KEY_RIGHT, {}}};

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
  if (action == GLFW_PRESS) {
    auto ipt = FindInputByCode(key);
    if (ipt != nullptr) {
      for (auto &b : ipt->bindings) {
        const auto c = CommandParser::FindCommand(b);
        c->func({});
      }
    }
  }
}

bool PC_Input::Init() {
  glfwSetKeyCallback(PC_Video::GetWindow(), key_callback);
  return true;
}

bool PC_Input::Update() {
  glfwPollEvents();
  if (glfwWindowShouldClose(PC_Video::GetWindow())) {
    CommandParser::Parse("quit"); // yuck
  }

  for (auto &it : actual_Inputs) {
    if (it.multiFirebindings.size() > 0 && (glfwGetKey(PC_Video::GetWindow(), it.code) == GLFW_PRESS)) {
      for (auto &mf : it.multiFirebindings) {
        const auto c = CommandParser::FindCommand(mf);
        c->func({});
      }
    }
  }
  for (auto &it : actual_Input_builtins) {
    if (it.multiFirebindings.size() > 0 && (glfwGetKey(PC_Video::GetWindow(), it.code) == GLFW_PRESS)) {
      for (auto &mf : it.multiFirebindings) {
        const auto c = CommandParser::FindCommand(mf);
        c->func({});
      }
    }
  }

  return true;
}

bool PC_Input::Shutdown() {
  glfwSetKeyCallback(PC_Video::GetWindow(), NULL);
  return true;
}

void Update(float delta) {}