#pragma once
#include "CommandParser.h"

class PC_Input {
public:
  static bool Init();
  static bool Update();
  static bool Shutdown();

protected:
private:
  PC_Input() = delete;
  ~PC_Input() = delete;
  // delete copy/move constructors & assign operators
  PC_Input(PC_Input const &) = delete;
  PC_Input(PC_Input &&) = delete;
  PC_Input &operator=(PC_Input const &) = delete;
  PC_Input &operator=(PC_Input &&) = delete;
};
