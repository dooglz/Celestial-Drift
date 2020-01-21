#pragma once

#include <sstream>
#include <stdio.h>
#include <string>

inline std::string NowTime();

enum TLogLevel { logERROR, logWARNING, logINFO, logDEBUG, logDEBUG1, logDEBUG2, logDEBUG3, logDEBUG4 };
enum TLogDest { logCONSOLe, logFILE, logBOTH };

#ifndef FILELOG_MAX_LEVEL
#define FILELOG_MAX_LEVEL logDEBUG4
#endif

class Logger {
public:
  Logger();
  ~Logger();

  std::ostringstream &Get(TLogLevel level = logINFO);

  static TLogLevel &ReportingLevel();
  static std::string ToString(TLogLevel level);
  static TLogLevel FromString(const std::string &level);
  static void SetDest(const TLogDest d);

protected:
  std::ostringstream os;
  static TLogDest dest;

private:
  // delete copy/move constructors & assign operators
  Logger(Logger const &) = delete;
  Logger(Logger &&) = delete;
  Logger &operator=(Logger const &) = delete;
  Logger &operator=(Logger &&) = delete;
};

#define LOG(level)                                                                                                     \
                                                                                                                       \
  if (level > FILELOG_MAX_LEVEL)                                                                                       \
    ;                                                                                                                  \
  else if (level > Logger::ReportingLevel())                                                                           \
    ;                                                                                                                  \
  else                                                                                                                 \
    Logger().Get(level)
