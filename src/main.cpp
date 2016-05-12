#include "CommandParser.h"
#include "common.h"
#include "cvar.h"
#include "font.h"
#include "gamelogic.h"
#include "input.h"
#include "platform.h"
#include "renderer.h"
#include "time.h"
#include "video.h"
#include <iostream>

using namespace std;

int main(int argc, char *argv[]) {
  cout << "hello world!" << endl;
  LOG(logDEBUG1) << "Big Red Engine - Commit: " << GIT_HASH << " - Date: " << GIT_DATE;

  // load configs

  // init platform
  Platform::Init();

  // init video subsystem
  Video::Init();

  // Init input subsystm
  Input::Init();

  // init Rendering subsystem
  Renderer::Init();

  Font::Init();
  // init Sound subsystem
  // init GameLogic
  GameLogic::Init();

  CommandParser::CmdParserStart();
  // Handover
  GameLogic::Run();
  CommandParser::CmdParserStop();

  GameLogic::Shutdown();
  Font::Shutdown();
  Renderer::Shutdown();
  Input::Shutdown();
  Video::Shutdown();

  LOG(logDEBUG1) << "Goodbye!";
}