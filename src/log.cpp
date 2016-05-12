#include "log.h"
#include "time.h"

Logger::Logger() {}
Logger::~Logger() {
  os << std::endl;
  fprintf(stderr, "%s", os.str().c_str());
  fflush(stderr);
}

TLogDest Logger::dest;

void Logger::SetDest(const TLogDest d) { dest = d; }

std::ostringstream &Logger::Get(TLogLevel level) {
  os << "-" << NowTimeString();
  os << "-" << ToString(level) << ": ";
  // os << std::string(level > logDEBUG ? level - logDEBUG : 0, '\t');
  return os;
}

TLogLevel &Logger::ReportingLevel() {
  static TLogLevel reportingLevel = logDEBUG4;
  return reportingLevel;
}

std::string Logger::ToString(TLogLevel level) {
  static const char *const buffer[] = {"ERROR", "WARNING", "INFO", "DEBUG", "DEBUG1", "DEBUG2", "DEBUG3", "DEBUG4"};
  return buffer[level];
}

TLogLevel Logger::FromString(const std::string &level) {
  if (level == "DEBUG4")
    return logDEBUG4;
  if (level == "DEBUG3")
    return logDEBUG3;
  if (level == "DEBUG2")
    return logDEBUG2;
  if (level == "DEBUG1")
    return logDEBUG1;
  if (level == "DEBUG")
    return logDEBUG;
  if (level == "INFO")
    return logINFO;
  if (level == "WARNING")
    return logWARNING;
  if (level == "ERROR")
    return logERROR;
  Logger().Get(logWARNING) << "Unknown logging level '" << level << "'. Using INFO level as default.";
  return logINFO;
}
