#include "component.h"

Component::Component(const std::string &token) {
  token_ = token;
  Ent_ = nullptr;
  active_ = false;
}

Component::~Component() {}

bool Component::IsActive() { return active_; }

void Component::SetActive(bool b) { active_ = b; }

void Component::SetParent(Entity *p) { Ent_ = p; }

Entity *Component::GetParent() { return Ent_; }
