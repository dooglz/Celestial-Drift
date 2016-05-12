
#pragma once
#include "common.h"
#include "component.h"
#include <glm/gtx/quaternion.hpp>
#include <glm\glm.hpp>

namespace Components {

class CmShipdriver : public Component {
public:
  glm::vec3 velocity_;
  glm::vec3 acceleration_;
  glm::vec3 angularAcc_;
  glm::vec3 angularVel_;
  void Accelerate(const float &f);
  void Brake(const float &f);
  void Steer(const float &f);

  CmShipdriver();
  ~CmShipdriver();

  void Update(double delta);

  uint32_t lapcount;
  uint32_t waypointIndex;
  uint32_t NextWaypointIndex;
  glm::vec3 NextWaypointRnd;
};
}
