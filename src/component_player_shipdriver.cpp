#include "component_player_shipdriver.h"
#include "CommandParser.h"
#include <iostream>

static Components::CmPlayerShipDriver *pp;

static bool player_accelerate(const std::vector<std::string> &params) {
  // what direction is player facing?
  pp->Accelerate(1.0f);
  return true;
}
static bool player_brake(const std::vector<std::string> &params) {
  pp->Brake(1.0f);
  return true;
}
static bool player_steer_left(const std::vector<std::string> &params) {
  pp->Steer(1.0f);
  return true;
}
static bool player_steer_right(const std::vector<std::string> &params) {
  pp->Steer(-1.0f);
  return true;
}

bool yolo(const std::vector<std::string> &params) {
  std::cout << std::endl;
  return 0;
}

namespace Components {
CmPlayerShipDriver::CmPlayerShipDriver() : CmShipdriver() {
  // Ben, if you looking at this terrible hack, this is your fault
  pp = this;

  CommandParser::commands.push_back({"player_accelerate", "", 1, player_accelerate});
  CommandParser::commands.push_back({"player_steer_left", "", 1, player_steer_left});
  CommandParser::commands.push_back({"player_steer_right", "", 1, player_steer_right});
  CommandParser::commands.push_back({"player_brake", "", 1, player_brake});

  CommandParser::Cmd_Bind({"", "player_accelerate", "W", "1"});
  CommandParser::Cmd_Bind({"", "player_brake", "S", "1"});
  CommandParser::Cmd_Bind({"", "player_steer_left", "A", "1"});
  CommandParser::Cmd_Bind({"", "player_steer_right", "D", "1"});
}
CmPlayerShipDriver::~CmPlayerShipDriver() { pp = nullptr; }
}