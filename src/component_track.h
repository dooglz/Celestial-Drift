#pragma once

#include "common.h"
#include "component.h"
#include "mesh.h"
#include "resource.h"

struct Mesh;

namespace Components {

class CmTrack : public Component {
protected:
  // RenderObject* _ro;
  Mesh *mesh_;

public:
  static Mesh *trackMesh;
  static Mesh *lineMesh;
  static std::vector<glm::vec3> waypoints;
  static std::vector<glm::vec3> transformedPositions;
  CmTrack();
  ~CmTrack();
  Mesh *getMesh();
  void SetMesh(Mesh *msh);
  void SetMesh(const std::string &meshname);
  void Update(double delta);
  void Moved();
  void Render();
};
}
