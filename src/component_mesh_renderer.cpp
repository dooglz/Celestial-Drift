#include "component_mesh_renderer.h"
#include "Entity.h"
#include "renderer.h"
#include "resource.h"
Components::CmMeshRenderer::CmMeshRenderer() : Component("MeshRenderer") {}

Components::CmMeshRenderer::~CmMeshRenderer() {}

void Components::CmMeshRenderer::SetMesh(Mesh *msh) { mesh_ = msh; }

void Components::CmMeshRenderer::SetMesh(const std::string &meshname) { SetMesh(Storage<Mesh>::Get(meshname)); }

void Components::CmMeshRenderer::Render() {
  if (!mesh_->loadedLocal) {
    Renderer::LoadMesh(mesh_);
  }
  Renderer::RenderMesh(*mesh_, Ent_->GetTranform());
  for (auto &m : mesh_->subMeshes) {
    if (!m->loadedLocal) {
      Renderer::LoadMesh(m);
    }
    Renderer::RenderMesh(*m, Ent_->GetTranform());
  }
}
