#pragma once
#include "platform.h"
#include <chrono>
#include <string>
using namespace std;

struct Timer {
  chrono::high_resolution_clock::time_point start;
  chrono::high_resolution_clock::time_point end;
  Timer();
  void Start();
  void Stop();
  const chrono::high_resolution_clock::duration Duration();
  unsigned long long Duration_NS();
};

std::string NowIsoString();
std::string NowDateTimeString();
std::string NowTimeString();

#if defined(_PLATFORM_X64) || defined(_PLATFORM_WIN32)
struct HighResClock {
  typedef long long rep;
  typedef std::nano period;
  typedef std::chrono::duration<rep, period> duration;
  typedef std::chrono::time_point<HighResClock> time_point;
  static const bool is_steady = true;

  static time_point now();
};

typedef HighResClock gameclock;
#else
typedef chrono::high_resolution_clock gameclock;
#endif
