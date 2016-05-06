#pragma once
#include <stdint.h>

#if defined(_PLATFORM_X64) || defined(_PLATFORM_WIN32)
#include "pc_platform.h"
class PC_Platform;
typedef PC_Platform Platform;
#elif defined(_PLATFORM_PS4)
#include "ps4_platform.h"
class PS4_Platform;
typedef PS4_Platform Platform;
#else

#endif

/*
class Platform {
public:
  static uint8_t GetPhysicalCores();
protected:

private:
  Platform() = delete;
  ~Platform() = delete;
  // delete copy/move constructors & assign operators
  Platform(Platform const &) = delete;
  Platform(Platform &&) = delete;
  Platform &operator=(Platform const &) = delete;
  Platform &operator=(Platform &&) = delete;
};
*/