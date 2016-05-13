#pragma once
#include "glm/glm.hpp"
#include <string>
#include <vector>

struct Mesh;
namespace Components {
class CmCamera;
}

class PC_Renderer {
public:
  static bool Init();
  static bool Shutdown();
  static void ClearFrame();
  static void ClearColour(glm::vec4 c);
  static void SetViewMatrix(const glm::mat4 &vm);
  static void SetProjectionMAtrix(const glm::mat4 &vm);
  static void ResetProjectionMatrix();
  static void RenderMesh(const Mesh &m, const glm::mat4 &modelMatrix);
  static void BindTexture(const unsigned int texID, const unsigned int texUnit, const std::string &shader);
  static void CreateSkybox(const std::string (&imgs)[6]);
  static void PC_Renderer::RenderSkybox();
  static void LoadMesh(Mesh *msh);
  static void PreRender() { ClearFrame(); };
  static void PostRender();
  static void DrawLine(const glm::vec3 &p1, const glm::vec3 &p2, const glm::vec4 &col1, const glm::vec4 &col2);
  static void DrawCross(const glm::vec3 &p1, const float size, const glm::vec4 &col);
  static void ProcessLines();

protected:
  static glm::mat4 vp_;
  static std::vector<std::pair<const glm::vec3, const glm::vec4>> linebuffer;

private:
  PC_Renderer() = delete;
  ~PC_Renderer() = delete;
  // delete copy/move constructors & assign operators
  PC_Renderer(PC_Renderer const &) = delete;
  PC_Renderer(PC_Renderer &&) = delete;
  PC_Renderer &operator=(PC_Renderer const &) = delete;
  PC_Renderer &operator=(PC_Renderer &&) = delete;
};
