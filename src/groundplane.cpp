#include "groundplane.h"
#include "mesh.h"
#include "renderer.h"
#include <glm/gtx/transform.hpp>
static Mesh plane_mesh;

void GroundPlane::Init() {
  plane_mesh = Mesh();
  plane_mesh.hasUvs = false;
  plane_mesh.hasNormals = false;
  plane_mesh.loadedLocal = false;
  plane_mesh.loadedMain = true;
  plane_mesh.hasColours = false;
  plane_mesh.hasIndicies = true;
  plane_mesh.numVerts = 4;
  plane_mesh.vertexData = {
      {-1.0f, 0.0f, 1.0f}, {-1.0f, 0.0f, -1.0f}, {1.0f, 0.0f, -1.0f}, {1.0f, 0.0f, 1.0f}};
  plane_mesh.indices = {0, 2, 1, 0, 3, 2};
  plane_mesh.shaderPref = "grid";
  Renderer::LoadMesh(&plane_mesh);
}

void GroundPlane::Shutdown() {}
void GroundPlane::Update() {}

void GroundPlane::Render(const float &x, const float &z) {
  glm::mat4 mm;
  mm = glm::translate(vec3(x, 0, z)) * glm::scale(vec3(50.0f));

  Renderer::RenderMesh(plane_mesh, mm);
}