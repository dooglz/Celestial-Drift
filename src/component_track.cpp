#include "component_track.h"
#include "Entity.h"
#include "renderer.h"
#include "resource.h"

Mesh *Components::CmTrack::trackMesh;
Mesh *Components::CmTrack::lineMesh;
std::vector<glm::vec3> Components::CmTrack::waypoints;
std::vector<glm::vec3> Components::CmTrack::transformedPositions;
Components::CmTrack::CmTrack() : Component("CmTrack") {}

Components::CmTrack::~CmTrack() {}

void Components::CmTrack::SetMesh(Mesh *msh) {
  mesh_ = msh;
  trackMesh = nullptr;
  lineMesh = nullptr;

  if (mesh_->objGroupName == "track") {
    trackMesh = mesh_;
  }
  if (mesh_->objGroupName == "BezierCurve") {
    lineMesh = mesh_;
  }

  for (auto &m : mesh_->subMeshes) {
    if (m->objGroupName == "track") {
      trackMesh = m;
    } else if (m->objGroupName == "BezierCurve") {
      lineMesh = m;
    }
  }

  if (trackMesh == nullptr || lineMesh == nullptr) {
    LOG(logERROR) << "Track has no colliisons or waypoints";
    return;
  }
  Moved();
}

void Components::CmTrack::SetMesh(const std::string &meshname) { SetMesh(Storage<Mesh>::Get(meshname)); }

void Components::CmTrack::Render() {
  if (!mesh_->loadedLocal) {
    Renderer::LoadMesh(mesh_);
  }
  if (mesh_->objGroupName != "BezierCurve") {
    Renderer::RenderMesh(*mesh_, Ent_->GetTranform());
  }

  for (auto &m : mesh_->subMeshes) {

    if (m->objGroupName == "BezierCurve") {
      continue;
    }

    if (!m->loadedLocal) {
      Renderer::LoadMesh(m);
    }
    Renderer::RenderMesh(*m, Ent_->GetTranform());
  }
  //
  Renderer::DrawCross(vec3(0, 0, 0), 5.0f, vec4(1.0f, 0, 0, 1.0f));
  Renderer::DrawCross(vec3(2, 2, 2), 5.0f, vec4(0, 1.0f, 0, 1.0f));
}

void Components::CmTrack::Update(double delta) {
  /*for (size_t i = 0; i < lineMesh->lines.size(); i += 2) {
    vec3 trn = vec3(Ent_->GetTranform() * vec4(lineMesh->vertexData[lineMesh->lines[i]], 1));
    Renderer::DrawCross(trn, 5.0f, vec4(1.0f, 0, 0, 1.0f));
  }*/
  for (size_t i = 0; i < transformedPositions.size(); i += 2) {
    // Renderer::DrawCross(transformedPositions[i], 5.0f, vec4(1.0f, 0, 1.0f, 1.0f));
  }
  for (size_t i = 0; i < waypoints.size(); ++i) {
    //  Renderer::DrawCross(waypoints[i], 5.0f, vec4(1.0f, 0, 0.0f, 1.0f));
  }
}

void Components::CmTrack::Moved() {
  if (trackMesh == nullptr || lineMesh == nullptr) {
    LOG(logERROR) << "Track has no colliisons or waypoints";
    return;
  }

  // call to prevent recusion data collision
  Ent_->GetTranform();

  waypoints.clear();
  for (size_t i = 0; i < lineMesh->lines.size(); i += 2) {
    waypoints.push_back(vec3(Ent_->GetTranform() * vec4(lineMesh->vertexData[lineMesh->lines[i]], 1)));
  }

  transformedPositions.clear();
  for (size_t i = 0; i < trackMesh->vertexData.size(); i++) {
    transformedPositions.push_back(vec3(Ent_->GetTranform() * vec4(trackMesh->vertexData[i], 1)));
  }
  int dd = 0;
}
