#include "CommandParser.h"
#include "cvar.h"
#include "input.h"
#include "log.h"
#include "scene.h"

#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

#if defined(_PLATFORM_X64) || defined(_PLATFORM_WIN32)
#include <conio.h>
#elif defined(_PLATFORM_PS4)
#endif

using namespace std;

const Command CommandParser::inbuilt_commands[]{{"help", "Show all commands", 0, Cmd_Help},
                                                {"get", "Get cvar (use * for list)", 1, Cmd_get},
                                                {"set", "Set cvar", 2, Cmd_set},
                                                {"reset", "Reset cvar to defaults", 1, Cmd_Reset},
                                                {"bind", "bind key to function", 3, Cmd_Bind},
                                                {"scene", "show scene info", 0, Cmd_Scene},
                                                {"listInputs", "shows all inputs and bindings", 0, Cmd_ListInputs}};

vector<Command> CommandParser::commands;

bool CommandParser::Parse(const std::string &command) {

  if (command.length() > 128) {
    LOG(logWARNING) << "Command String too large ";
    return false;
  }

  // LOG(logDEBUG2) << "Parsing command: " << command;

  std::stringstream ss(command);
  std::vector<string> vect;
  std::string s;

  size_t token_count = 0;
  size_t max_tokens = 16;
  const Command *cmd = nullptr;

  while (ss >> s && token_count < max_tokens + 1) {
    vect.push_back(s);

    if (ss.peek() == ' ') {
      ss.ignore();
      if (token_count == 0) {
        cmd = FindCommand(s);
        if (cmd == nullptr) {
          LOG(logWARNING) << "Invalid command";
          return false;
        }
        max_tokens = cmd->params;
      }
    }
    ++token_count;
  }

  if (cmd == nullptr) {
    // There were no parameters
    cmd = FindCommand(s);
    if (cmd == nullptr) {
      LOG(logWARNING) << "Invalid command";
      return false;
    }
  } else if (token_count < max_tokens) {
    LOG(logWARNING) << "Missing Command Parameters " << cmd->description;
    return false;
  }

  if (cmd->func != 0) {
    return cmd->func(vect);
  } else {
    LOG(logWARNING) << "Unimplmented command: " << cmd->name << " - " << cmd->description;
  }
  return true;
}

const Command *CommandParser::FindCommand(const std::string &commandName) {

  for (auto &it : inbuilt_commands) {
    if (it.name == commandName) {
      return &it;
    }
  }
  for (auto &it : commands) {
    if (it.name == commandName) {
      return &it;
    }
  }
  return nullptr;
}

bool CommandParser::Cmd_get(const std::vector<std::string> &params) {
  if (params[1] == "*") {
    std::ostringstream ss;
    ss << "\r\n";
    for (auto &it : ConVar::cvarList) {
      ss << it->GetName() << "\t" << it->GetString() << "\r\n";
    }
    LOG(logINFO) << ss.str();
    return true;
  }

  const ConVar *const c = ConVar::FindCvar(params[1]);
  if (c == nullptr) {
    LOG(logWARNING) << "Invalid Cvar: " << params[1];
    return false;
  }
  LOG(logINFO) << c->GetString();

  return true;
}

bool CommandParser::Cmd_set(const std::vector<std::string> &params) {
  ConVar *const c = ConVar::FindCvar(params[1]);
  if (c == nullptr) {
    LOG(logWARNING) << "Invalid Cvar: " << params[1];
    return false;
  }

  c->SetValue(params[2].c_str());
  return true;
}

bool CommandParser::Cmd_Reset(const std::vector<std::string> &params) {
  ConVar *const c = ConVar::FindCvar(params[1]);
  if (c == nullptr) {
    LOG(logWARNING) << "Invalid Cvar: " << params[1];
    return false;
  }

  c->Revert();
  return true;
}

bool CommandParser::Cmd_ListInputs(const std::vector<std::string> &params) {
  ListInputs();
  return true;
}

bool CommandParser::Cmd_Help(const std::vector<std::string> &) {
  std::ostringstream ss;
  ss << "\r\n";
  for (auto &it : CommandParser::inbuilt_commands) {
    ss << it.name << "\t" << it.description << "\r\n";
  }
  for (auto &it : CommandParser::commands) {
    ss << it.name << "\t" << it.description << "\r\n";
  }
  LOG(logINFO) << ss.str();
  return true;
}

static bool parser_is_go;
static std::mutex mtx;

static void ParseThread() {
#if defined(_PLATFORM_X64) || defined(_PLATFORM_WIN32)
  while (true) {
    mtx.lock();
    if (!parser_is_go) {
      mtx.unlock();
      break;
    }
    mtx.unlock();

    if (_kbhit()) {
      std::string name;
      std::getline(std::cin, name);
      CommandParser::Parse(name);
    }
  }
#elif defined(_PLATFORM_PS4)
#endif
}

bool CommandParser::Cmd_Bind(const std::vector<std::string> &params) {
  const Command *c = CommandParser::FindCommand(params[1]);
  actual_Input *i = FindInput(params[2]);
  bool multifire = (params.size() > 3 && params[3] != "");
  // const Command cc = {};
  if (i == nullptr) {
    LOG(logWARNING) << "Invalid input: " << params[2];
    return false;
  }
  if (c == nullptr) {
    LOG(logWARNING) << "Invalid command: " << params[2];
    return false;
  }
  if (multifire) {
    i->multiFirebindings.push_back(params[1]);
  } else {
    i->bindings.push_back(params[1]);
  }
  return true;
}

std::thread first;
void CommandParser::CmdParserStart() {
#if defined(_PLATFORM_X64) || defined(_PLATFORM_WIN32)
  mtx.lock();
  parser_is_go = true;
  mtx.unlock();
  first = std::thread(ParseThread);
#elif defined(_PLATFORM_PS4)
#endif
}

void CommandParser::CmdParserStop() {
#if defined(_PLATFORM_X64) || defined(_PLATFORM_WIN32)
  mtx.lock();
  parser_is_go = false;
  mtx.unlock();

  first.join(); // will wait for user input
#elif defined(_PLATFORM_PS4)
#endif
}

Command::Command(const std::string &pname, const std::string &pdescription, const uint8_t pparams,
                 const std::function<bool(const std::vector<std::string> &param)> pfunc)
    : name(pname), description(pdescription), params(pparams), func(pfunc) {

  cout << endl;
}

/*
Command::Command(const char *pname, const char *pdescription, const uint8_t pparams,
                 const FnCommandCallback_t pfunc)
    : params(pparams), func(pfunc) {
  name = pname;
  description = pdescription;
}
*/
bool CommandParser::Cmd_Scene(const std::vector<std::string> &params) {
  Scene::Debug();
  return true;
}