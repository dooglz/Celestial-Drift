#include "component_npc_shipdriver.h"
#include "CommandParser.h"
#include "component_track.h"
#include "Entity.h"
#include "gamelogic.h"
#include <iostream>
#include <glm/gtx/vector_angle.hpp>

namespace Components {
CmNPCShipDriver::CmNPCShipDriver() : CmShipdriver() {}

CmNPCShipDriver::~CmNPCShipDriver() {}

void CmNPCShipDriver::Update(double delta) {
  if (GameLogic::racestate != RUNNING || GameLogic::gamestate != GAME) {
    return;
  }


  CmShipdriver::Update(delta);
  const glm::vec3 cpos = Components::CmTrack::waypoints[waypointIndex];
  const glm::vec3 tpos = NextWaypointRnd; //Components::CmTrack::waypoints[NextWaypointIndex];
  const glm::vec3 vcpos = cpos - Ent_->GetPosition();
  const glm::vec3 vtpos = tpos - Ent_->GetPosition();
  // const glm::vec3 vforward = GetForwardVector(Ent_->GetRotation());
  const glm::vec3 vforward = Ent_->GetRotation() * vec3(0, 0, 1.0f);
  const float ang =
      glm::orientedAngle(glm::normalize(vforward), glm::normalize(vtpos), vec3(0, 1.0f, 0));
  const float anga = abs(ang);
  
  if (ang > 0.1f) {
    Steer(1.0f);
  } else if (ang < -0.1f) {
    Steer(-1.0f);
  }

  if (anga > 1.5f) {
    Brake(1.0f);
  } else {
    float acc = min((1.5f - anga) / 1.0f, 1.0f);
    Accelerate(acc);
  }
}
}