#pragma once
#include <stdint.h>

#if defined(__FreeBSD__) || defined(__FreeBSD_kernel__) || defined(__DragonFly__)
#define _PLATFORM_FREEBSD
#elif defined(__APPLE__)
/* lets us know what version of Mac OS X we're compiling on */
#include "AvailabilityMacros.h"
#include "TargetConditionals.h"
#if TARGET_OS_IPHONE
/* if compiling for iPhone */
#define _PLATFORM_IPHONEOS
#else
/* if not compiling for iPhone */
#define _PLATFORM_MACOSX
#endif
#elif defined(linux) || defined(__linux) || defined(__linux__)
#define _PLATFORM_LINUX
#elif defined(ANDROID)
#define _PLATFORM_ANDROID
#elif defined(WIN32) || defined(_WIN32) || defined(_WIN64)
#define _PLATFORM_WINDOWS
#if defined(_WIN64)
#define _PLATFORM_X64
#elif defined(_WIN32)
#define _PLATFORM_WIN32
#endif
#elif defined(__PSP__)
#define _PLATFORM_PSP
#elif defined(__psp2__)
#define _PLATFORM_VITA
#elif defined(__PPU__) || defined(__SPU__)
#define _PLATFORM_PS3
#elif defined(__ORBIS__)
#define _PLATFORM_PS4
#elif defined(__XBOX360__)
#define _PLATFORM_360
#elif defined(__DURANGO__)
#define _PLATFORM_XBONE
#endif

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