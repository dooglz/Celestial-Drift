#include "component_camera.h"
#include "entity.h"
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
  return glm::lookAt(lastpos_,
                     Ent_->GetPosition(), // and looks at the origin
                     glm::vec3(0, 1, 0)   // Head is up (set to 0,-1,0 to look upside-down)
                     );
}

void Components::FollowCamera::Update(double delta) {
  vec3 f = normalize(GetForwardVector(Ent_->GetRotation()));
  vec3 pos = Ent_->GetPosition() + (-10.0f * f) + vec3(0, 4.0f, 0);
  lastpos_ = glm::mix(lastpos_, pos, 4.0 * delta);
}

//--------
Components::FlyCamera::FlyCamera(){}

Components::FlyCamera::~FlyCamera() {}

bool Components::FlyCamera::IsActive() { return false; }

void Components::FlyCamera::Move(Direction d) {

  const float speed = 1.0f;
  const float hRotSpeed = 0.05f * speed;
  const float vRotSpeed = 0.035f * speed;
  vec3 rot = vec3(0);

  {
    const vec3 UP = vec3(0, 1.0f, 0);
    const vec3 RIGHT = vec3(0, 0, 1.0f);
    const vec3 FORWARD = vec3(1.0f, 0, 0);
    // pitch
    if (d == PD) {
      Ent_->SetRotation(angleAxis(vRotSpeed, rotate(inverse(Ent_->GetRotation()), FORWARD)) *
                        Ent_->GetRotation());
    }
    if (d == PU) {
      // Ent->setRotation(AngleAxisToQuat(FORWARD*Inverse(Ent->getRotation()),
      // -vRotSpeed) * Ent->getRotation());
      Ent_->SetRotation(angleAxis(-vRotSpeed, rotate(inverse(Ent_->GetRotation()), FORWARD)) *
                        Ent_->GetRotation());
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

  vec3 right = normalize(GetRightVector(Ent_->GetRotation()));
  vec3 direction = normalize(GetForwardVector(Ent_->GetRotation()));

  // First person fly camera movement
  rot = vec3(0);
  if (d == SR) {
    rot -= (speed)*right;
  }
  if (d == SL) {
    rot += (speed)*right;
  }
  if (d == BACK) {
    rot -= (speed)*direction;
  }
  if (d == FORWARD) {
    rot += (speed)*direction;
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
