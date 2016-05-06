#pragma once
#include "common.h"
enum Gamestate { INIT, MAINMENU, GAME, PAUSE, SHUTDOWN };
enum RaceState { UNINIT,READY, RUNNING, END };

class GameLogic {
public:
  static Gamestate gamestate;
  static RaceState racestate;
  static bool Init();
  static bool Run();
  static bool Shutdown();

protected:
private:
  GameLogic() = delete;
  ~GameLogic() = delete;
  // delete copy/move constructors & assign operators
  GameLogic(GameLogic const &) = delete;
  GameLogic(GameLogic &&) = delete;
  GameLogic &operator=(GameLogic const &) = delete;
  GameLogic &operator=(GameLogic &&) = delete;
};