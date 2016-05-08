#include "CommandParser.h"
#include "Physics.h"
#include "common.h"
#include "component_camera.h"
#include "component_mesh_renderer.h"
#include "component_npc_shipdriver.h"
#include "component_player_shipdriver.h"
#include "component_shipdriver.h"
#include "component_track.h"
#include "entity.h"
#include "gamelogic.h"
#include "genericInput.h"
#include "glm/glm.hpp"
#include "groundplane.h"
#include "input.h"
#include "menu.h"
#include "renderer.h"
#include "resource.h"
#include "scene.h"
#include "ship.h"
#include "time.h"
#include "video.h"
#include <glm/gtx/transform.hpp>
#include <iomanip>
#include <iostream>
// SUPER TEMP
#include "font.h"
bool RUNGAME;
bool helpselect;
bool controlselect;
bool inMenu;

string controls = "";

const string versionText = "WIP BUILD:" + version::Hash + " - " +
                           string(version::CommitDate, 0, version::CommitDate.length() - 6);

Gamestate GameLogic::gamestate;
RaceState GameLogic::racestate;

#define SHIPCOUNT 16
Entity ships[SHIPCOUNT];
Entity blankEnt;

Entity player;
Entity npc;
Entity trackEnt;

bool GameLogic::Init() {
  gamestate = INIT;
  racestate = UNINIT;
  GroundPlane::Init();
  RUNGAME = true;
  inMenu = true;
  return false;
}

bool Menu_up(const std::vector<std::string> &params) {
  LOG(logDEBUG2) << "Menu Up";
  Menu::activeMenu->Move(MENU_UP);
  return true;
}
bool Menu_down(const std::vector<std::string> &params) {
  LOG(logDEBUG2) << "Menu Down";
  Menu::activeMenu->Move(MENU_DOWN);
  return true;
}
bool Menu_enter(const std::vector<std::string> &params) {
  LOG(logDEBUG2) << "Menu Enter";
  Menu::activeMenu->Move(MENU_RIGHT);
  return true;
}

bool Start_Game(const std::vector<std::string> &params) {
  LOG(logDEBUG2) << "starting game";
  inMenu = false;
  return true;
}
bool Quit_game(const std::vector<std::string> &params) {
  RUNGAME = false;
  return true;
}
bool Menu_Help(const std::vector<std::string> &params) {
  helpselect = true;
  return true;
}
bool Back_To_Menu(const std::vector<std::string> &params) {
  LOG(logDEBUG2) << "main menu";
  helpselect = false;
  return true;
}

bool Controls(const std::vector<std::string> &params) {
  for (auto &it : actual_Input_builtins) {
    for (auto &b : it.bindings) {
      controls += std::string(b) + ", " + std::string(it.name) + '\n';
    }
  }

  if (controlselect != true) {
    controlselect = true;
  } else {
    controlselect = false;
  }
  return true;
}

bool GameLogic::Run() {
  const vec3 start = glm::vec3(-7, 8, -30);
  for (size_t i = 0; i < SHIPCOUNT; i++) {
    int row = (i+1) % 2;
    int collumn = (i+1) / 2;
    float spacing = 4.0f;

    if (i == 0) {
      BuildShipPlayer(&ships[i]);
    } else {
      BuildShipNpc(&ships[i]);
    }
    ships[i].SetPosition(start + (vec3(collumn, 0, row) * spacing));
    ships[i].SetRotation(glm::vec3(0, 1.5708f, 0));
    Scene::scene_list.push_back(&ships[i]);
  }

  Components::FollowCamera cam = Components::FollowCamera();
  ships[0].AddComponent(cam);
  //Scene::SetActiveCamera(&cam);

  Components::FlyCamera fcam = Components::FlyCamera();
  blankEnt = Entity();
  blankEnt.AddComponent(fcam);
  Scene::SetActiveCamera(&fcam);


  trackEnt = Entity();
  Components::CmTrack trackmr = Components::CmTrack();
  trackEnt.AddComponent(trackmr);
  trackmr.SetMesh("track.obj");
  trackEnt.SetPosition(glm::vec3(0, 0.5f, 0));
  trackEnt.SetScale(glm::vec3(200.0f));

  Scene::scene_list.push_back(&trackEnt);

  // main menu setup
  auto mnu = new Menu();
  mnu->SetPosition({400, 400});
  auto mnuitm1 = new MenuItem("Celestial Drift", false, nullptr);
  mnu->GetItems()->push_back(mnuitm1);
  auto mnuitm2 = new MenuItem("Start", true, Start_Game);
  mnu->GetItems()->push_back(mnuitm2);
  auto mnuitm3 = new MenuItem("Help", true, Menu_Help);
  mnu->GetItems()->push_back(mnuitm3);
  auto mnuitm4 = new MenuItem("Quit", true, Quit_game);
  mnu->GetItems()->push_back(mnuitm4);

  auto helpmnu = new Menu();
  helpmnu->SetPosition({100, 100});
  auto helpmnuitm1 = new MenuItem("Celestial Drift Help Menu", false, nullptr);
  helpmnu->GetItems()->push_back(helpmnuitm1);
  auto helpmnuitm2 = new MenuItem("Start", true, Start_Game);
  helpmnu->GetItems()->push_back(helpmnuitm2);
  auto helpmnuitm3 = new MenuItem("Back To Menu", true, Back_To_Menu);
  helpmnu->GetItems()->push_back(helpmnuitm3);
  auto helpmnuitm4 = new MenuItem("Quit", true, Quit_game);
  helpmnu->GetItems()->push_back(helpmnuitm4);
  auto helpmnuitm5 = new MenuItem("Controls", true, Controls);
  helpmnu->GetItems()->push_back(helpmnuitm5);

  // input
  CommandParser::commands.push_back({"menu_up", "", 0, Menu_up});
  CommandParser::commands.push_back({"menu_down", "", 0, Menu_down});
  CommandParser::commands.push_back({"menu_enter", "", 0, Menu_enter});
  CommandParser::commands.push_back({"quit", "", 0, Quit_game});
  CommandParser::commands.push_back({"Controls", "", 0, Controls});
  CommandParser::commands.push_back({"Back_To_Menu", "", 0, Back_To_Menu});
  CommandParser::commands.push_back({"Start_Game", "", 0, Start_Game});
  CommandParser::commands.push_back({"Menu_Help", "", 0, Menu_Help});

  // key bindings, route through the parser for now
  CommandParser::Cmd_Bind({"", "menu_up", "UP", ""});
  CommandParser::Cmd_Bind({"", "menu_down", "DOWN", ""});
  CommandParser::Cmd_Bind({"", "menu_enter", "ENTER", ""});
  CommandParser::Cmd_Bind({"", "quit", "ESC", ""});

  Mesh texm = Mesh();
  texm.hasUvs = true;
  texm.hasNormals = false;
  texm.loadedLocal = false;
  texm.loadedMain = true;
  texm.hasColours = false;
  texm.hasIndicies = true;
  texm.numVerts = 4;
  texm.vertexData = {
      {-1.0f, 1.0f, 0.0f}, {-1.0f, -1.0f, 0.0f}, {1.0f, -1.0f, 0.0f}, {1.0f, 1.0f, 0.0f}
  };
  texm.uvs = { {-1.0f, 1.0f },{ -1.0f, -1.0f },{ 1.0f, -1.0f },{ 1.0f, 1.0f }};
  texm.indices = { 0, 2, 1, 0, 3, 2 };
  texm.shaderPref = "grid";
  Renderer::LoadMesh(&texm);




  auto t = gameclock::now();
  double frametimes[256];
  for (auto &t : frametimes) {
    t = 0.0;
  }
  uint8_t ftc = 0;
  gamestate = MAINMENU;
  while (RUNGAME) {
    double delta = std::chrono::duration<double>(gameclock::now() - t).count();
    if (delta <= 0.0) {
      delta = 0.000000000001;
      LOG(logDEBUG) << "Delta of 0!";
    }
    // const double deltaPercent = (1.0 / 60.0) / delta;

    t = gameclock::now();
    static float runtime = 0;
    runtime += (float)delta;
    frametimes[++ftc] = delta;
    static string avg = "FPS:0";
    if (ftc % 10 == 0) {
      double davg = 0;
      for (const auto t : frametimes) {
        davg += t;
      }
      davg = 1 / (davg / 255.0);
      avg = "FPS:" + toStrDecPt(2, davg);
    }

    Renderer::ClearColour(glm::vec4(((sin((0.1f * runtime) + 0) * 127.0f) + 50.0f) / 255.0f,
                                    ((sin((0.1f * runtime) + 2) * 127.0f) + 50.0f) / 255.0f,
                                    ((sin((0.1f * runtime) + 4) * 127.0f) + 50.0f) / 255.0f, 1.0f));

    Input::Update();

    // menu choice
    if (inMenu) {
      if (helpselect) {
        Menu::activeMenu = helpmnu;
        helpmnu->Update();
      } else {
        Menu::activeMenu = mnu;
        mnu->Update();
      }
    } else {
      gamestate = GAME;
      racestate = RUNNING;
    }

    for (size_t i = 0; i < Components::CmTrack::trackMesh->indices.size(); i += 3) {
      vec3 p[3];
      p[0] = Components::CmTrack::transformedPositions[Components::CmTrack::trackMesh->indices[i]];
      p[1] =
          Components::CmTrack::transformedPositions[Components::CmTrack::trackMesh->indices[i + 1]];
      p[2] =
          Components::CmTrack::transformedPositions[Components::CmTrack::trackMesh->indices[i + 2]];
      vec3 point;
      vec3 resultNormal;

      float depth;

      for (size_t i = 0; i < SHIPCOUNT; i++) {
        //ship track collision
        if (Collide(ships[i].GetPosition(), 1.0f, p, point, resultNormal, depth)) {
          std::vector<Component *> cc = ships[i].GetComponents("ShipDriver");
          Resolve(ships[i], *(Components::CmShipdriver *)(cc[0]), resultNormal, depth);
        }
      }
    }
    for (size_t i = 0; i < SHIPCOUNT; i++) {
      for (size_t j = 0; j < SHIPCOUNT; j++) {
        if (j == i) { continue; }
        //ship ship collision
        vec3 point;
        vec3 resultNormal;
        float depth;
        if (Collide(ships[i].GetPosition(), 1.0f, ships[j].GetPosition(), 1.0f, point, resultNormal, depth)) {
       //   cout << i << " " << j << tos(point) << tos(resultNormal) << depth << endl;
          std::vector<Component *> cc = ships[i].GetComponents("ShipDriver");
          Resolve(ships[i], *(Components::CmShipdriver *)(cc[0]), resultNormal, depth);
        }
      }
    }

    const uint32_t plapcount =
        ((Components::CmShipdriver *)(ships[0].GetComponents("ShipDriver")[0]))->lapcount;
    const uint32_t pwaypointIndex =
        ((Components::CmShipdriver *)(ships[0].GetComponents("ShipDriver")[0]))->waypointIndex;
    uint32_t position = 1;

    for (size_t i = 1; i < SHIPCOUNT; i++) {
      const uint32_t lapcount =
          ((Components::CmShipdriver *)(ships[i].GetComponents("ShipDriver")[0]))->lapcount;
      if (lapcount > plapcount) {
        ++position;
      } else if (lapcount == plapcount) {
        const uint32_t waypointIndex =
            ((Components::CmShipdriver *)(ships[i].GetComponents("ShipDriver")[0]))->waypointIndex;
        if (waypointIndex > pwaypointIndex) {
          ++position;
        }
      }
    }

    Scene::Update(delta);

    GroundPlane::Update();
    Renderer::PreRender();

    Scene::Render(delta);
    //GroundPlane::Render(player.GetPosition().x, player.GetPosition().z);

    Renderer::RenderMesh(texm, mat4());

    // rendering choice
    if (inMenu) {
      if (helpselect == true) {
        helpmnu->Render();
      } else {
        mnu->Render();
      }
    }

    Font::Draw(25, versionText.c_str(), {300, 30}, {1.0f, 1.0f, 0, 1.0f});
    Font::Draw(25, avg.c_str(), {100, 30}, {0.2f, 0, 0, 1.0f});
    stringstream strs;
    strs << "Position: " << position;
    Font::Draw(25, strs.str().c_str(), {100, 100}, {0.2f, 0, 0, 1.0f});
    strs.str("");
    strs << "Lap: " << plapcount;
    Font::Draw(25, strs.str().c_str(), {100, 150}, {0.2f, 0, 0, 1.0f});

    // renders in the same place as help text so stop that
    if (helpselect == false) {
      /*  stringstream speedstream;

      speedstream << fixed << std::setprecision(2) << playerspeedReal << " m/s\n("
                  << player.GetPosition().x << "," << player.GetPosition().z << ")";
      Font::Draw(25, speedstream.str().c_str(), {100, 200}, {0.2f, 0, 0, 1.0f});
      */
    }
    if (controlselect == true && helpselect == true) {
      Font::Draw(25, controls.c_str(), {400, 400}, {0.2f, 1.0f, 0, 1.0f});
    } else {
      controls = "";
    }
    Font::Render();
    Renderer::PostRender();
    Video::Swap();
  }
  GroundPlane::Shutdown();
  return true;
}

bool GameLogic::Shutdown() { return false; }
