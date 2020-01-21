#include "component_player_shipdriver.h"
#include "CommandParser.h"
#include <iostream>
static int ttp = 0;
using namespace std;

namespace Components {
CmPlayerShipDriver::CmPlayerShipDriver(const unsigned int playerID) : playerID_(playerID), CmShipdriver() {
  CommandParser::commands.push_back(
      {"player_" + to_string(playerID) + "_accelerate", "", 1, [this](const vector<string> &params) {
         Accelerate(1.0f);
         return true;
       }});
  CommandParser::commands.push_back(
      {"player_" + to_string(playerID) + "_steer_left", "", 1, [this](const vector<string> &params) {
         Steer(1.0f);
         return true;
       }});
  CommandParser::commands.push_back(
      {"player_" + to_string(playerID) + "_steer_right", "", 1, [this](const vector<string> &params) {
         Steer(-1.0f);
         return true;
       }});
  CommandParser::commands.push_back(
      {"player_" + to_string(playerID) + "_brake", "", 1, [this](const vector<string> &params) {
         Brake(1.0f);
         return true;
       }});

  if (playerID == 0) {
    CommandParser::Cmd_Bind({"", "player_" + to_string(playerID) + "_accelerate", "W", "1"});
    CommandParser::Cmd_Bind({"", "player_" + to_string(playerID) + "_brake", "S", "1"});
    CommandParser::Cmd_Bind({"", "player_" + to_string(playerID) + "_steer_left", "A", "1"});
    CommandParser::Cmd_Bind({"", "player_" + to_string(playerID) + "_steer_right", "D", "1"});
  } else {
    CommandParser::Cmd_Bind({"", "player_" + to_string(playerID) + "_accelerate", "NP8", "1"});
    CommandParser::Cmd_Bind({"", "player_" + to_string(playerID) + "_brake", "S", "NP5"});
    CommandParser::Cmd_Bind({"", "player_" + to_string(playerID) + "_steer_left", "NP4", "1"});
    CommandParser::Cmd_Bind({"", "player_" + to_string(playerID) + "_steer_right", "NP6", "1"});
  }
}
CmPlayerShipDriver::~CmPlayerShipDriver() {}
} // namespace Components
