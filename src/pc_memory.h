#pragma once
#include "common.h"

class PC_Memory {
public:
  static void PrintReport();
  static uint64_t GetTotalMemory();

protected:
private:
  PC_Memory() = delete;
  ~PC_Memory() = delete;
  // delete copy/move constructors & assign operators
  PC_Memory(PC_Memory const &) = delete;
  PC_Memory(PC_Memory &&) = delete;
  PC_Memory &operator=(PC_Memory const &) = delete;
  PC_Memory &operator=(PC_Memory &&) = delete;
};