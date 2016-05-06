#include "component_camera.h"
#include "entity.h"
#include "glm\glm.hpp"
#include <glm/gtx/transform.hpp>

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
Components::FollowCamera::FollowCamera() : Component("FollowCamera") { lastpos_ = {0, 0, 0}; }

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