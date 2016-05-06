#include "CommandParser.h"
#include "input.h"
#include "genericInput.h"

std::vector<actual_Input> actual_Inputs;

actual_Input *FindInputByCode(const uint32_t &code) {

  for (auto &it : actual_Inputs) {
    if (it.code == code) {
      return &it;
    }
  }
  for (auto &it : actual_Input_builtins) {
    if (it.code == code) {
      return &it;
    }
  }
  return nullptr;
}
actual_Input *FindInput(const std::string &inputName) {

  for (auto &it : actual_Inputs) {
    if (it.name == inputName) {
      return &it;
    }
  }
  for (auto &it : actual_Input_builtins) {
    if (it.name == inputName) {
      return &it;
    }
  }
  return nullptr;
}

void ListInputs() {
  for (auto &it : actual_Input_builtins) {
    std::string s = "";
    for (auto &b : it.bindings) {
      s += std::string(b) + ", ";
    }
    for (auto &b : it.multiFirebindings) {
      s += std::string(b) + "(MF), ";
    }
    LOG(logINFO) << it.name << " (" << it.code << ")  - " << s;
  }
  for (auto &it : actual_Inputs) {
    std::string s = "";
    for (auto &b : it.bindings) {
      s += std::string(b) + ", ";
    }
    for (auto &b : it.multiFirebindings) {
      s += std::string(b) + "(MF), ";
    }
    LOG(logINFO) << it.name << " (" << it.code << ")  - " << s;
  }
}
