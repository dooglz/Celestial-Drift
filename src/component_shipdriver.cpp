#include "Entity.h"
#include "component_shipdriver.h"
#include "component_track.h"
#include "gamelogic.h"
#include "renderer.h"

using namespace glm;

namespace Components {

CmShipdriver::CmShipdriver() : Component("ShipDriver") {

  velocity_ = vec3(0.0f, 0.0f, 0.0f);
  acceleration_ = vec3(0.0f, 0.0f, 0.0f);
  angularVel_ = vec3(0.0f, 0.0f, 0.0f);
  angularAcc_ = vec3(0.0f, 0.0f, 0.0f);
  lapcount = 0;
  waypointIndex = 0;
  NextWaypointIndex = 0;
  NextWaypointRnd = vec3(0);
}

CmShipdriver::~CmShipdriver() {}

const float angularDamp_factor = 50.0f;
const float angularAcc_factor = 5.0f;
const float velocityDamp_factor = 2.0f;
const float velocityAcc_factor = 30.0f;

void CmShipdriver::Update(double delta) {
  if (GameLogic::racestate != RUNNING || GameLogic::gamestate != GAME) {
    return;
  }

  if (Ent_->GetPosition().y < -30.0f){
    Ent_->SetPosition(Components::CmTrack::waypoints[waypointIndex] + vec3(0,2.0f,0));
    velocity_ = vec3(0.0f, 0.0f, 0.0f);
    acceleration_ = vec3(0.0f, 0.0f, 0.0f);
    angularVel_ = vec3(0.0f, 0.0f, 0.0f);
    angularAcc_ = vec3(0.0f, 0.0f, 0.0f);
  }

  // Gravity
  acceleration_ += vec3(0, -9.8f, 0);

  const float dt = (float)delta;
  const auto playerspeed = glm::length(velocity_);
  const auto playerRotspeed = glm::length(angularVel_);
  // const auto playerspeedReal = glm::length(velocity_) / delta;
  // air resistance
  if (playerspeed != 0.0f) {
    const float dampening = 0.1f;
    velocity_ *= pow((1.0 / velocityDamp_factor), dt);

    // LOG(logERROR) << playerspeed;
    // acceleration_ += (-1.0f * playerspeed * normalize(velocity_));
    if (abs(velocity_.y) < 0.0001f) {
      velocity_.y = 0.0f;
    }
    if (abs(velocity_.x) < 0.0001f) {
      velocity_.x = 0.0f;
    }
    if (abs(velocity_.z) < 0.001f) {
      velocity_.z = 0.0f;
    }
  }

  if (playerRotspeed != 0.0f) {

    const float dampening = 0.1f;
    angularVel_ *= pow((1.0 / angularDamp_factor), dt);

    if (abs(angularVel_.y) < 0.0001f) {
      angularVel_.y = 0.0f;
    }
    if (abs(angularVel_.x) < 0.0001f) {
      angularVel_.x = 0.0f;
    }
    if (abs(angularVel_.z) < 0.001f) {
      angularVel_.z = 0.0f;
    }
  }

  velocity_ += acceleration_ * dt;
  acceleration_ = vec3(0.0f);
  angularVel_ += angularAcc_ * dt;
  angularAcc_ = vec3(0.0f);
  Ent_->SetPosition(Ent_->GetPosition() + velocity_ * dt);
  Ent_->SetRotation(Ent_->GetRotation() * glm::quat(angularVel_ * dt));

  // find the nerest waypoint
  if (!Components::CmTrack::waypoints.size()) {
    // no track nav info,
    return;
  }

  float cd = glm::length(Components::CmTrack::waypoints[waypointIndex] - Ent_->GetPosition());
  for (size_t i = 0; i < Components::CmTrack::waypoints.size(); ++i) {
    if (i != waypointIndex) {
      float nd = glm::length(Components::CmTrack::waypoints[i] - Ent_->GetPosition());
      if (nd < cd) {

        // LOG(logDEBUG1) << i << " " << cd << " "<<nd;
        cd = nd;
        waypointIndex = i;
        NextWaypointIndex = ((waypointIndex + 1) % (Components::CmTrack::waypoints.size()));
        NextWaypointRnd = Components::CmTrack::waypoints[NextWaypointIndex] +
                          ((vec3(Frand() - 0.5f, 0, Frand() - 0.5f)) * 15.0f);
        //  LOG(logDEBUG1) << waypointIndex << " " << NextWaypointIndex;
      }
    }
  }
  //Renderer::DrawCross(NextWaypointRnd, 4.0f, vec4(1.0f, 0.5f, 1.0f, 1.0f));

  // Renderer::DrawLine(Ent_->GetPosition(), Components::CmTrack::waypoints[waypointIndex],
  // vec4(1.0f, 0.5f, 1.0f, 1.0f), vec4(1.0f, 0.5f, 1.0f, 1.0f));
  // Renderer::DrawLine(Ent_->GetPosition(), Components::CmTrack::waypoints[NextWaypointIndex],
  // vec4(1.0f, 1.0f, 0.0f, 1.0f), vec4(1.0f, 0.5f, 1.0f, 1.0f));
}

void CmShipdriver::Accelerate(const float &f) {
  vec3 direction = normalize(GetForwardVector(Ent_->GetRotation()));
  acceleration_ += (velocityAcc_factor * direction * f);
}
void CmShipdriver::Brake(const float &f) {
  if (glm::length(velocity_) > 0) {
    acceleration_ += (-0.1f * velocity_ * f);
  }
}
void CmShipdriver::Steer(const float &f) { angularAcc_ += vec3(0, angularAcc_factor * f, 0); }
}
