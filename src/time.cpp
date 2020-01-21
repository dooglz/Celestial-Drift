#include "time.h"
#include <ctime>

#if defined(_PLATFORM_X64) || defined(_PLATFORM_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

std::string NowIsoString() {
  const std::time_t t = std::time(nullptr);
  char c[28];
  tm tme;
#if defined(_PLATFORM_X64) || defined(_PLATFORM_WIN32)
  gmtime_s(&tme, &t);
  strftime(c, 28, "%FT%T%z", &tme);
#elif defined(_PLATFORM_PS4)
  gmtime_s(&t, &tme);
  strftime(c, 28, "%FT%T%z", &tme);
#else
  gmtime_r(&t, &tme); // TODO:Test This
  strftime(c, 28, "%FT%T%z", &tme);
#endif // _MSC_VER

  return string(c);
}

std::string NowDateTimeString() {
  const std::time_t t = std::time(nullptr);
  char c[18];
  tm tme;
#if defined(_PLATFORM_X64) || defined(_PLATFORM_WIN32)
  gmtime_s(&tme, &t);
  strftime(c, 18, "%y-%m-%d_%T", &tme);
#elif defined(_PLATFORM_PS4)
  gmtime_s(&t, &tme);
  strftime(c, 18, "%y-%m-%d_%T", &tme);
#else
  gmtime_r(&t, &tme); // TODO:Test This
  strftime(c, 18, "%y-%m-%d_%T", &tme);
#endif // _MSC_VER
  return string(c);
}

std::string NowTimeString() {
  const std::time_t t = std::time(nullptr);
  char c[9];
  tm tme;
#if defined(_PLATFORM_X64) || defined(_PLATFORM_WIN32)
  gmtime_s(&tme, &t);
  strftime(c, 9, "%X", &tme);
#elif defined(_PLATFORM_PS4)
  gmtime_s(&t, &tme);
  strftime(c, 9, "%T", &tme);
#else
  gmtime_r(&t, &tme); // TODO:Test This
  strftime(c, 9, "%T", &tme);
#endif // _MSC_VER

  const auto currentTime = std::chrono::system_clock::now();
  time_t time = std::chrono::system_clock::to_time_t(currentTime);
  auto currentTimeRounded = std::chrono::system_clock::from_time_t(time);
  if (currentTimeRounded > currentTime) {
    --time;
    currentTimeRounded -= std::chrono::seconds(1);
  }
  const unsigned int milliseconds =
      std::chrono::duration_cast<std::chrono::duration<int, std::milli>>(currentTime - currentTimeRounded).count();
  return string(c) + ":" + to_string(milliseconds) + (milliseconds < 100 ? "0" : "") + (milliseconds < 10 ? "0" : "");
}

Timer::Timer() { Start(); }
void Timer::Start() { start = chrono::high_resolution_clock::now(); }
void Timer::Stop() { end = chrono::high_resolution_clock::now(); }
const chrono::high_resolution_clock::duration Timer::Duration() { return end - start; }
unsigned long long Timer::Duration_NS() { return chrono::duration_cast<chrono::nanoseconds>(Duration()).count(); };

#if defined(_PLATFORM_X64) || defined(_PLATFORM_WIN32)
namespace {
const long long g_Frequency = []() -> long long {
  LARGE_INTEGER frequency;
  QueryPerformanceFrequency(&frequency);
  return frequency.QuadPart;
}();
}

HighResClock::time_point HighResClock::now() {
  LARGE_INTEGER count;
  QueryPerformanceCounter(&count);
  return time_point(duration(count.QuadPart * static_cast<rep>(period::den) / g_Frequency));
}

#else

#endif
