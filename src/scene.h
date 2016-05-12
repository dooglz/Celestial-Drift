#pragma once
#include <vector>
class Entity;

namespace Components {
class CmCamera;
class FollowCamera;
}

class Scene {
public:
  static std::vector<Entity *> scene_list;
  static void Update(double delta);
  static void Render(double delta);
  static void SetActiveCamera(Components::CmCamera *c);
  // static void SetActiveCamera(Components::FollowCamera *fc);
  static void Debug();

private:
  static Components::CmCamera *activeCamera_;
  static Components::FollowCamera *playerCamera_;
  Scene() = delete;
  ~Scene() = delete;
  Scene(Scene const &) = delete;
  Scene(Scene &&) = delete;
  Scene &operator=(Scene const &) = delete;
  Scene &operator=(Scene &&) = delete;
};
