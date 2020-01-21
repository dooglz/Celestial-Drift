#include "CommandParser.h"
#include "Physics.h"
#include "TextureManager.h"
#include "common.h"
#include "component_camera.h"
#include "component_mesh_renderer.h"
#include "component_npc_shipdriver.h"
#include "component_player_shipdriver.h"
#include "component_shipdriver.h"
#include "component_track.h"
#ifdef SOUND
#include "pc_sound.h"
#endif
#include "entity.h"
#include "filesystem.h"
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
bool debugcmra;
bool multiplayer;

const string versionText =
    "WIP BUILD:" + string(GIT_HASH) + " - " + string(string(GIT_DATE), 0, string(GIT_DATE).length() - 6);

Gamestate GameLogic::gamestate;
RaceState GameLogic::racestate;

#define SHIPCOUNT 16
Entity ships[SHIPCOUNT];
Entity powerups[4];
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
  multiplayer = false;
  return false;
}

void ToggleMP() {
  multiplayer = !multiplayer;
  if (multiplayer) {
    auto &p2 = ships[1];
    p2.RemoveComponent(*p2.getComponent<Components::CmNPCShipDriver>());
    Components::CmPlayerShipDriver *dr2 = new Components::CmPlayerShipDriver(1);
    p2.AddComponent(*dr2);
    Components::FollowCamera *cam2 = new Components::FollowCamera();
    p2.AddComponent(*cam2);
  } else {
    auto &p2 = ships[1];
    p2.RemoveComponent(*p2.getComponent<Components::CmPlayerShipDriver>());
    p2.RemoveComponent(*p2.getComponent<Components::FollowCamera>());
    Components::CmNPCShipDriver *dr2 = new Components::CmNPCShipDriver();
    p2.AddComponent(*dr2);
  }
}

bool ToggleFly(const std::vector<std::string> &params) {
  debugcmra = !debugcmra;
  return true;
}

bool GameLogic::Run() {
  debugcmra = false;
  const vec3 start = glm::vec3(-7, 8, -30);

  for (size_t i = 0; i < SHIPCOUNT; i++) {
    int row = (i + 1) % 2;
    int collumn = (i + 1) / 2;
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
  // Scene::SetActiveCamera(&cam);

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
  trackmr.Moved();

  Scene::scene_list.push_back(&trackEnt);

  for (size_t i = 0; i < 4; i++) {
    BuildPowerup(&powerups[i]);
    Scene::scene_list.push_back(&powerups[i]);
  }

  // main menu setup
  auto mnu = new Menu();
  mnu->SetPosition({400, 400});
  auto mnuitm1 = new MenuItem("Celestial Drift", false, nullptr);

  mnu->GetItems()->push_back(mnuitm1);
  auto mnuitm2 = new MenuItem("Start", true, [](const vector<string> &params) {
    inMenu = false;
    return true;
  });
  mnu->GetItems()->push_back(mnuitm2);

  auto mnuitm3 = new MenuItem("Help", true, [](const vector<string> &params) {
    helpselect = true;
    return true;
  });
  mnu->GetItems()->push_back(mnuitm3);

  auto mnuitm4 = new MenuItem("Quit", true, [](const vector<string> &params) {
    RUNGAME = false;
    return true;
  });
  mnu->GetItems()->push_back(mnuitm4);

  auto helpmnu = new Menu();
  helpmnu->SetPosition({100, 100});
  auto helpmnuitm1 = new MenuItem("Celestial Drift Help Menu", false, nullptr);
  helpmnu->GetItems()->push_back(helpmnuitm1);
  auto helpmnuitm3 = new MenuItem("Back To Menu", true, [](const vector<string> &params) {
    helpselect = false;
    controlselect = false;
    return true;
  });
  helpmnu->GetItems()->push_back(helpmnuitm3);
  auto helpmnuitm5 = new MenuItem("Controls", true, [](const vector<string> &params) {
    controlselect = !controlselect;
    return true;
  });
  helpmnu->GetItems()->push_back(helpmnuitm5);

  // input
  CommandParser::commands.push_back({"toggle_MP", "", 0, [](const vector<string> &params) {
                                       ToggleMP();
                                       return true;
                                     }});

  CommandParser::commands.push_back({"toggle_fly", "", 0, [](const vector<string> &params) {
                                       debugcmra = !debugcmra;
                                       return true;
                                     }});

  CommandParser::commands.push_back({"menu_up", "", 0, [](const vector<string> &params) {
                                       Menu::activeMenu->Move(MENU_UP);
                                       return true;
                                     }});
  CommandParser::commands.push_back({"menu_down", "", 0, [](const vector<string> &params) {
                                       Menu::activeMenu->Move(MENU_DOWN);
                                       return true;
                                     }});
  CommandParser::commands.push_back({"menu_enter", "", 0, [](const vector<string> &params) {
                                       Menu::activeMenu->Move(MENU_RIGHT);
                                       return true;
                                     }});

  CommandParser::commands.push_back({"quit", "", 0, [](const vector<string> &params) {
                                       RUNGAME = false;
                                       return true;
                                     }});

  // key bindings, route through the parser for now
  CommandParser::Cmd_Bind({"", "menu_up", "UP", ""});
  CommandParser::Cmd_Bind({"", "menu_down", "DOWN", ""});
  CommandParser::Cmd_Bind({"", "menu_enter", "ENTER", ""});
  CommandParser::Cmd_Bind({"", "quit", "ESC", ""});
  CommandParser::Cmd_Bind({"", "toggle_fly", "V", ""});
  CommandParser::Cmd_Bind({"", "toggle_MP", "M", ""});

  Renderer::CreateSkybox({"resources/img/bk.jpg", "resources/img/ft.jpg", "resources/img/up.jpg",
                          "resources/img/dn.jpg", "resources/img/lf.jpg", "resources/img/rt.jpg"});

#ifdef SOUND
  PC_Sound::Init();
#endif
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

    uint32_t position = 1;
    uint32_t plapcount = 0;
    uint32_t position2 = 1;
    uint32_t plapcount2 = 0;

    Renderer::ClearColour(glm::vec4(((sin((0.1f * runtime) + 0) * 127.0f) + 50.0f) / 255.0f,
                                    ((sin((0.1f * runtime) + 2) * 127.0f) + 50.0f) / 255.0f,
                                    ((sin((0.1f * runtime) + 4) * 127.0f) + 50.0f) / 255.0f, 1.0f));

    Input::Update();
    if (debugcmra) {
      Scene::SetActiveCamera(&fcam);
    } else {
      Scene::SetActiveCamera(&cam);
    }

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

      for (size_t i = 0; i < Components::CmTrack::trackMesh->indices.size(); i += 3) {
        vec3 p[3];
        p[0] = Components::CmTrack::transformedPositions[Components::CmTrack::trackMesh->indices[i]];
        p[1] = Components::CmTrack::transformedPositions[Components::CmTrack::trackMesh->indices[i + 1]];
        p[2] = Components::CmTrack::transformedPositions[Components::CmTrack::trackMesh->indices[i + 2]];
        vec3 point;
        vec3 resultNormal;

        float depth;

        for (size_t i = 0; i < SHIPCOUNT; i++) {
          // ship track collision
          if (Collide(ships[i].GetPosition(), 1.0f, p, point, resultNormal, depth)) {
            std::vector<Component *> cc = ships[i].GetComponents("ShipDriver");
            Resolve(ships[i], *(Components::CmShipdriver *)(cc[0]), resultNormal, depth);
          }
        }
      }
      for (size_t i = 0; i < SHIPCOUNT; i++) {
        for (size_t j = 0; j < SHIPCOUNT; j++) {
          if (j == i) {
            continue;
          }
          // ship ship collision
          vec3 point;
          vec3 resultNormal;
          float depth;
          if (Collide(ships[i].GetPosition(), 1.0f, ships[j].GetPosition(), 1.0f, point, resultNormal, depth)) {
            if (i == 0 || j == 0) {
#ifdef SOUND
              PC_Sound::bump();
#endif
            }
            //   cout << i << " " << j << tos(point) << tos(resultNormal) << depth << endl;
            std::vector<Component *> cc = ships[i].GetComponents("ShipDriver");
            Resolve(ships[i], *(Components::CmShipdriver *)(cc[0]), resultNormal, depth);
          }
        }
      }

      plapcount = ((Components::CmShipdriver *)(ships[0].GetComponents("ShipDriver")[0]))->lapcount;
      plapcount2 = ((Components::CmShipdriver *)(ships[1].GetComponents("ShipDriver")[0]))->lapcount;
      const uint32_t pwaypointIndex =
          ((Components::CmShipdriver *)(ships[0].GetComponents("ShipDriver")[0]))->waypointIndex;
      const uint32_t pwaypointIndex2 =
          ((Components::CmShipdriver *)(ships[1].GetComponents("ShipDriver")[0]))->waypointIndex;
      position = 1;
      position2 = 1;
      for (size_t i = 1; i < SHIPCOUNT; i++) {
        const uint32_t lapcount = ((Components::CmShipdriver *)(ships[i].GetComponents("ShipDriver")[0]))->lapcount;
        if (lapcount > plapcount) {
          ++position;
        } else if (lapcount == plapcount) {
          const uint32_t waypointIndex =
              ((Components::CmShipdriver *)(ships[i].GetComponents("ShipDriver")[0]))->waypointIndex;
          if (waypointIndex > pwaypointIndex) {
            ++position;
          }
        }
        if (lapcount > plapcount2) {
          ++position2;
        } else if (lapcount == plapcount2) {
          const uint32_t waypointIndex =
              ((Components::CmShipdriver *)(ships[i].GetComponents("ShipDriver")[0]))->waypointIndex;
          if (waypointIndex > pwaypointIndex2) {
            ++position2;
          }
        }
      }
      Font::Draw(25, versionText.c_str(), {300, 30}, {1.0f, 1.0f, 0, 1.0f});
      Font::Draw(25, avg.c_str(), {100, 30}, {0.2f, 0, 0, 1.0f});
    }
    Scene::Update(delta);

    GroundPlane::Update();
    Renderer::PreRender();

    if (multiplayer) {
      PC_Video::SplitviewPort(2, 0);
      PC_Renderer::SetProjectionMAtrix(glm::perspective(
          glm::radians(45.0f), (float)DEFAULT_RESOLUTION_X / (((float)DEFAULT_RESOLUTION_Y) * 0.5f), 0.1f, 1000.0f));
      Scene::SetActiveCamera(ships[0].getComponent<Components::FollowCamera>());
      Scene::Render(delta);
      PC_Renderer::RenderSkybox();
      if (racestate == RUNNING) {
        stringstream strs;
        strs << "Position: " << position2;
        Font::Draw(25, strs.str().c_str(), {100, 100}, {0.9f, 0.2f, 0.2f, 1.0f});
        strs.str("");
        strs << "Lap: " << plapcount2;
        Font::Draw(25, strs.str().c_str(), {100, 150}, {0.9f, 0.2f, 0.2f, 1.0f});
      }
      PC_Video::SplitviewPort(2, 1);
      Scene::SetActiveCamera(ships[1].getComponent<Components::FollowCamera>());
      Scene::Render(delta);
      PC_Renderer::RenderSkybox();
      if (racestate == RUNNING) {
        stringstream strs;
        strs << "Position: " << position;
        Font::Draw(25, strs.str().c_str(), {100, 500}, {0.9f, 0.2f, 0.2f, 1.0f});
        strs.str("");
        strs << "Lap: " << plapcount;
        Font::Draw(25, strs.str().c_str(), {100, 550}, {0.9f, 0.2f, 0.2f, 1.0f});
      }
      PC_Video::SplitviewPort(0, 0);
      PC_Renderer::ResetProjectionMatrix();
    } else {
      PC_Video::SplitviewPort(0, 0);
      PC_Renderer::ResetProjectionMatrix();
      Scene::SetActiveCamera(ships[0].getComponent<Components::FollowCamera>());
      Scene::Render(delta);
      PC_Renderer::RenderSkybox();
      if (racestate == RUNNING) {
        stringstream strs;
        strs << "Position: " << position;
        Font::Draw(25, strs.str().c_str(), {100, 100}, {0.9f, 0.2f, 0.2f, 1.0f});
        strs.str("");
        strs << "Lap: " << plapcount;
        Font::Draw(25, strs.str().c_str(), {100, 150}, {0.9f, 0.2f, 0.2f, 1.0f});
      }
    }
    // GroundPlane::Render(player.GetPosition().x, player.GetPosition().z);

    // rendering choice
    if (inMenu) {
      if (helpselect == true) {
        helpmnu->Render();
      } else {
        mnu->Render();
      }
    }

    if (controlselect == true) {
      string controls;
      for (auto &it : actual_Input_builtins) {
        for (auto &b : it.bindings) {
          controls += std::string(b) + ", " + std::string(it.name) + '\n';
        }
      }
      Font::Draw(25, controls.c_str(), {400, 400}, {0.2f, 1.0f, 0, 1.0f});
    }

    Font::Render();
    Renderer::PostRender();
    Video::Swap();
  }
  GroundPlane::Shutdown();
  return true;
}

bool GameLogic::Shutdown() { return false; }
