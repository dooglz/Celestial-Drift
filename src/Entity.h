#pragma once
#include "Component.h"
#include "common.h"
#include "maths.h"
#include <glm/gtc/quaternion.hpp>
#include <string>
#include <vector>

class Component;

class Entity {
protected:
  bool visible_;
  std::string name_;
  bool changed_;
  vec3 scale_;
  vec3 position_;
  quat rotation_;
  mat4 transform_;
  // void CalculateTransform();
  std::vector<Component *> components_;

public:
  Entity();
  ~Entity();

  const vec3 GetScale() const;
  const vec3 GetPosition() const;
  const vec3 GetRotationV3() const;
  const quat GetRotation() const;
  const mat4 GetTranform();
  const bool IsVisible() const;
  const std::string GetName() const;

  void SetTransform(const mat4 m4);
  void SetScale(const vec3 &v3);
  void SetPosition(const vec3 &v3);
  void SetRotation(const vec3 &v3);
  void SetRotation(const quat &q);

  void SetVisibility(const bool b);
  void SetName(std::string const &name);

  virtual void Update(const double delta);
  virtual void Render();

  void AddComponent(Component &c);
  void RemoveComponent(Component &c);
  const std::vector<Component *> GetComponents() const;
  std::vector<Component *> GetComponents(std::string const &name) const;

  template <typename T> T *const getComponent() {
    for (std::vector<Component *>::iterator it = components_.begin(); it != components_.end(); ++it) {
      // printf("Checking %s against %s \n", typeid(**it).name(),
      // typeid(T).name());
      if (&typeid(**it) == &typeid(T)) {
        return static_cast<T *>(*it);
      }
    }
    return NULL;
  }
};