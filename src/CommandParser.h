#pragma once
#include "common.h"
#include <string>
#include <functional>
#include <vector>

// typedef bool (*FnCommandCallback_t)(const std::vector<std::string> &params);
struct Command {
  const std::string name;
  const std::string description;
  const uint8_t params;
  const std::function<bool(const std::vector<std::string> &param)> func;

  // const std::function<bool(const std::vector<std::string> &param)> func;

  Command(const std::string &pname, const std::string &pdescription, const uint8_t pparams,
          const std::function<bool(const std::vector<std::string> &param)> pfunc);
};

class CommandParser {
public:
  static bool Parse(const std::string &command);
  static const Command *FindCommand(const std::string &commandName);
  static const Command inbuilt_commands[];
  static std::vector<Command> commands;
  static void CmdParserStart();
  static void CmdParserStop();
  // cmds
  static bool Cmd_get(const std::vector<std::string> &params);
  static bool Cmd_set(const std::vector<std::string> &params);
  static bool Cmd_Reset(const std::vector<std::string> &params);
  static bool Cmd_Help(const std::vector<std::string> &);
  static bool Cmd_Bind(const std::vector<std::string> &params);
  static bool Cmd_Scene(const std::vector<std::string> &params);
  static bool Cmd_ListInputs(const std::vector<std::string> &params);
  //
protected:
private:
  CommandParser() = delete;
  ~CommandParser() = delete;
  // delete copy/move constructors & assign operators
  CommandParser(CommandParser const &) = delete;
  CommandParser(CommandParser &&) = delete;
  CommandParser &operator=(CommandParser const &) = delete;
  CommandParser &operator=(CommandParser &&) = delete;
};
