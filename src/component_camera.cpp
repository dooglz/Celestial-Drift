#include "component_camera.h"
#include "entity.h"
#include "CommandParser.h"
#include "glm\glm.hpp"
#include <glm/gtx/transform.hpp>
#include "glm/gtx/euler_angles.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"
#include "glm/gtc/matrix_transform.hpp"

Components::CmCamera::CmCamera() : Component("Camera") {}

Components::CmCamera::~CmCamera() {}

bool Components::CmCamera::IsActive() { return false; }

glm::mat4 Components::CmCamera::getViewMatrix() {
  return glm::lookAt(Ent_->GetPosition(), // Camera is at (4,3,-3), in World Space
                     glm::vec3(0, 0, 0),  // and looks at the origin
                     glm::vec3(0, 1, 0)   // Head is up (set to 0,-1,0 to look upside-down)
                     );
}

// followcamera setup
Components::FollowCamera::FollowCamera(){ lastpos_ = {0, 0, 0}; }

Components::FollowCamera::~FollowCamera() {}

bool Components::FollowCamera::IsActive() { return false; }

glm::mat4 Components::FollowCamera::getViewMatrix() {
  vec3 f = normalize(GetForwardVector(Ent_->GetRotation()));
  return glm::lookAt(lastpos_,
                     Ent_->GetPosition()+(f*10.0f), // and looks at the origin
                     glm::vec3(0, 1, 0)   // Head is up (set to 0,-1,0 to look upside-down)
                     );
}

void Components::FollowCamera::Update(double delta) {
  vec3 f = normalize(GetForwardVector(Ent_->GetRotation()));
  vec3 pos = Ent_->GetPosition() + (-8.0f * f) + vec3(0, 3.0f, 0);
  lastpos_ = glm::mix(lastpos_, pos, 4.0 * delta);

}

//--------
static Components::FlyCamera *pp;
//todo lamba this mess
static bool f_(const std::vector<std::string> &params) {
  pp->Move(Components::FlyCamera::FORWARD);
  return true;
}
static bool b_(const std::vector<std::string> &params) {
  pp->Move(Components::FlyCamera::BACK);
  return true;
}
static bool sl_(const std::vector<std::string> &params) {
  pp->Move(Components::FlyCamera::SL);
  return true;
}
static bool sr_(const std::vector<std::string> &params) {
  pp->Move(Components::FlyCamera::SR);
  return true;
}
static bool pu_(const std::vector<std::string> &params) {
  pp->Move(Components::FlyCamera::PU);
  return true;
}
static bool pd_(const std::vector<std::string> &params) {
  pp->Move(Components::FlyCamera::PD);
  return true;
}
static bool pl_(const std::vector<std::string> &params) {
  pp->Move(Components::FlyCamera::PL);
  return true;
}
static bool pr_(const std::vector<std::string> &params) {
  pp->Move(Components::FlyCamera::PR);
  return true;
}

Components::FlyCamera::FlyCamera(){
  CommandParser::commands.push_back({ "f_", "", 1, f_ });
  CommandParser::commands.push_back({ "b_", "", 1, b_ });
  CommandParser::commands.push_back({ "sl_", "", 1, sl_ });
  CommandParser::commands.push_back({ "sr_", "", 1, sr_ });
  CommandParser::commands.push_back({ "pu_", "", 1, pu_ });
  CommandParser::commands.push_back({ "pd_", "", 1, pd_ });
  CommandParser::commands.push_back({ "pl_", "", 1, pl_ });
  CommandParser::commands.push_back({ "pr_", "", 1, pr_ });

  CommandParser::Cmd_Bind({ "", "f_", "W", "1" });
  CommandParser::Cmd_Bind({ "", "b_", "S", "1" });
  CommandParser::Cmd_Bind({ "", "sl_", "A", "1" });
  CommandParser::Cmd_Bind({ "", "sr_", "D", "1" });
  CommandParser::Cmd_Bind({ "", "pu_", "UP", "1" });
  CommandParser::Cmd_Bind({ "", "pd_", "DOWN", "1" });
  CommandParser::Cmd_Bind({ "", "pl_", "LEFT", "1" });
  CommandParser::Cmd_Bind({ "", "pr_", "RIGHT", "1" });
  pp = this; //haha oh wow such bad, much spagetti
}

Components::FlyCamera::~FlyCamera() {}

bool Components::FlyCamera::IsActive() { return false; }

void Components::FlyCamera::Move(Direction d) {

  const float speed = 0.2f;
  const float hRotSpeed = 0.05f * speed;
  const float vRotSpeed = 0.035f * speed;
  vec3 rot = vec3(0);
  const vec3 right = normalize(GetRightVector(Ent_->GetRotation()));
  const vec3 forward = normalize(GetForwardVector(Ent_->GetRotation()));

  {
    const vec3 UP = vec3(0, 1.0f, 0);
    // pitch
    if (d == PD) {
      //Ent_->SetRotation(angleAxis(vRotSpeed, rotate(inverse(Ent_->GetRotation()), FORWARD)) * Ent_->GetRotation());
      Ent_->SetRotation(angleAxis(vRotSpeed, right) * Ent_->GetRotation());
    }
    if (d == PU) {
      // Ent->setRotation(AngleAxisToQuat(FORWARD*Inverse(Ent->getRotation()),
      // -vRotSpeed) * Ent->getRotation());
      //Ent_->SetRotation(angleAxis(-vRotSpeed, rotate(inverse(Ent_->GetRotation()), FORWARD)) * Ent_->GetRotation());
      Ent_->SetRotation(angleAxis(-vRotSpeed, right) * Ent_->GetRotation());
    }

    // yaw
    if (d == PL) {
      Ent_->SetRotation(angleAxis(hRotSpeed, UP) * Ent_->GetRotation());
    }
    if (d == PR) {
      Ent_->SetRotation(angleAxis(-hRotSpeed, UP) * Ent_->GetRotation());
    }
  }
  Ent_->SetRotation(normalize(Ent_->GetRotation()));

  // First person fly camera movement
  rot = vec3(0);
  if (d == SR) {
    rot -= (speed)*right;
  }
  if (d == SL) {
    rot += (speed)*right;
  }
  if (d == BACK) {
    rot -= (speed)*forward;
  }
  if (d == FORWARD) {
    rot += (speed)*forward;
  }

  Ent_->SetPosition(Ent_->GetPosition() + rot);
}

glm::mat4 Components::FlyCamera::getViewMatrix() {
  quat cq = Ent_->GetRotation();
  vec3 pos = Ent_->GetPosition();

  vec3 up = normalize(GetUpVector(Ent_->GetRotation()));
  vec3 forwards = normalize(GetForwardVector(cq));

  return glm::lookAt(pos, pos + forwards, up);
}

void Components::FlyCamera::Update(double delta) {}
