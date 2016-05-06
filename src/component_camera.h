#pragma once
#include "common.h"
#include "component.h"
#include "glm\glm.hpp"
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
  glm::mat4 getViewMatrix();
  bool IsActive();
};

class FollowCamera : public Component {
protected:
  glm::vec3 lastpos_;

public:
  FollowCamera();
  ~FollowCamera();
  glm::mat4 getViewMatrix();
  bool IsActive();
  void Update(double delta);
};
}
