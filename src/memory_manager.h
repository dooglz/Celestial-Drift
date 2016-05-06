#pragma once
#include "common.h"

#if defined(_PLATFORM_X64) || defined(_PLATFORM_WIN32)
#include "pc_memory.h"
class PC_Memory;
typedef PC_Memory MemoryManager;
#elif defined(_PLATFORM_PS4)
#include "ps4_memory.h"
class PS4_Memory;
typedef PS4_Memory MemoryManager;
#else

#endif

/*
class MemoryManager {
public:
  static void PrintReport();
  static uint64_t GetTotalMemory();
protected:

private:
  MemoryManager() = delete;
  ~MemoryManager() = delete;
  // delete copy/move constructors & assign operators
  MemoryManager(MemoryManager const &) = delete;
  MemoryManager(MemoryManager &&) = delete;
  MemoryManager &operator=(MemoryManager const &) = delete;
  MemoryManager &operator=(MemoryManager &&) = delete;
};
*/