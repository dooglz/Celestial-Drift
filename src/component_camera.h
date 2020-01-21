#pragma once
#include "common.h"
#include "component.h"
#include "glm/glm.hpp"
// struct Texture;
// enum Materials;
// struct RenderObject;
namespace Components {

class CmCamera : public Component {
protected:
  // RenderObject* _ro;

public:
  CmCamera();
  ~CmCamera();
  virtual glm::mat4 getViewMatrix();
  bool IsActive();
};

class FollowCamera : public CmCamera {
protected:
  glm::vec3 lastpos_;

public:
  FollowCamera();
  ~FollowCamera();
  glm::mat4 getViewMatrix();
  bool IsActive();
  void Update(double delta);
};
class FlyCamera : public CmCamera {
protected:
public:
  enum Direction { FORWARD, BACK, SL, SR, PU, PD, PL, PR };
  FlyCamera();
  ~FlyCamera();
  glm::mat4 getViewMatrix();
  bool IsActive();
  void Move(Direction d);
  void Update(double delta);
};
}
