#pragma once

#if defined(_PLATFORM_X64) || defined(WIN32)
#include "pc_video.h"
class PC_Video;
typedef PC_Video Video;
#elif defined(_PLATFORM_PS4)
#include "ps4_video.h"
class PS4_Video;
typedef PS4_Video Video;
#else

#endif
/*
class Video {
public:
  static bool Init();
  static bool Resize();
  static bool Shutdown();
protected:

private:
  Video() = delete;
  ~Video() = delete;
  // delete copy/move constructors & assign operators
  Video(Video const &) = delete;
  Video(Video &&) = delete;
  Video &operator=(Video const &) = delete;
  Video &operator=(Video &&) = delete;
};
*/