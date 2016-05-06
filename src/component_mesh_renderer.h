#pragma once

#include "common.h"
#include "component.h"
#include "resource.h"
#include "mesh.h"

struct Mesh;
// struct Texture;
// enum Materials;
// struct RenderObject;
namespace Components {

class CmMeshRenderer : public Component {
protected:
  // RenderObject* _ro;
  Mesh *mesh_;

public:
  CmMeshRenderer();
  ~CmMeshRenderer();

  Mesh *getMesh();
  void SetMesh(Mesh *msh);
  void SetMesh(const std::string &meshname);

  // void SetMaterial(const std::string& materialName);
  // void SetMaterial(Materials mat);
  // void* GetMaterial();
  // void SetMaterialTexture(unsigned int i, const std::string& textureName);
  // void SetMaterialTexture(unsigned int i, Texture* tex);
  // void SetMaterialParameter(unsigned int i, void* param);
  void Render();
};
}
