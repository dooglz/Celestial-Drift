#pragma once
#include "CommandParser.h"
#include "common.h"

// why does this headder exists? because I can't design good software

struct actual_Input {
  const char *name;
  uint32_t code;
  std::vector<std::string> bindings;
  std::vector<std::string> multiFirebindings;
};

extern std::vector<actual_Input> actual_Inputs;
extern std::vector<actual_Input> actual_Input_builtins;
actual_Input *FindInput(const std::string &inputName);
actual_Input *FindInputByCode(const uint32_t &code);
void ListInputs();