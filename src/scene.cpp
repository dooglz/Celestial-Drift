#include "scene.h"
#include "component_camera.h"
#include "Entity.h"
#include "renderer.h"
#include <string>
std::vector<Entity *> Scene::scene_list;
Components::CmCamera *Scene::activeCamera_;
// Components::FollowCamera *Scene::playerCamera_;

// void Scene::SetActiveCamera(Components::FollowCamera *fc) { playerCamera_ = fc; }

void Scene::SetActiveCamera(Components::CmCamera *c) { activeCamera_ = c; }

void Scene::Update(double delta) {
  for (auto &e : scene_list) {
    e->Update(delta);
  }
}

void Scene::Render(double delta) {

  // should really be doing sorting and culling
  // Calulate visibility
  auto cc = activeCamera_->getViewMatrix();
  Renderer::SetViewMatrix(activeCamera_->getViewMatrix());

  for (auto &e : scene_list) {
    e->Render();
  }
}

void Scene::Debug() {
  for (auto &e : scene_list) {

    std::string comps = "";
    for (auto &c : e->GetComponents()) {
      comps += "\n\t " + c->token_;
    }
    LOG(logDEBUG) << e->GetName() << " \t " << tos(e->GetPosition()) << " \t " << tos(e->GetRotationV3()) << comps;
  }
}
