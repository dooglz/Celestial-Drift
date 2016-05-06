#pragma once
#include "common.h"
#include <string>

class Entity;
class Component {
protected:
  // Parent Entity
  Entity *Ent_;
  bool active_;

public:
  std::string token_;
  Component(const std::string &token);
  virtual ~Component();
  virtual void Update(double delta){};
  // Note: the Ent will only call moved when it's transform is requested
  virtual void Moved(){};
  virtual void Render(){};
  bool IsActive();
  void SetActive(bool b);
  void SetParent(Entity *p);
  Entity *GetParent();
};