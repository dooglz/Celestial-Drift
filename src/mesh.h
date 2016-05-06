#pragma once
#include "common.h"
#include "maths.h"
#include <string>
#include <vector>
//

#if defined(_PLATFORM_X64) || defined(_PLATFORM_WIN32)
#elif defined(_PLATFORM_PS4)
#include <gnm.h>

typedef struct Vertex {
  float x, y, z; // Position
  float r, g, b; // Color
  float u, v;    // UVs
} Vertex;
typedef struct VertexC {
  float x, y, z; // Position
  float r, g, b; // Color
} VertexC;
typedef struct VertexP {
  float x, y, z; // Positions
} VertexP;

enum VertexElements { kVertexPosition = 0, kVertexColor, kVertexUv, kVertexElemCount };
enum VertexCElements { kVertexCPosition = 0, kVertexCColor, kVertexCElemCount };
enum VertexPElements { kVertexPPosition = 0, kVertexPElemCount };
#endif

struct Mesh {
  static Mesh *Load(const std::string &name);
  std::string name;
  std::string objGroupName;
  std::string shaderPref;
  uint32_t numVerts;
  bool hasUvs;
  bool hasNormals;
  bool hasColours;
  bool hasIndicies;
  bool hasLines;
  bool strip;
  bool fan;
  bool line;
  bool loadedMain;
  bool loadedLocal;

  // mesh data in main
  std::vector<Mesh *> subMeshes;
  std::vector<vec3> vertexData;
  std::vector<unsigned int> colours;
  std::vector<vec2> uvs;
  std::vector<vec3> normals;
  std::vector<uint32_t> indices;
  std::vector<uint32_t> lines;

#if defined(_PLATFORM_X64) || defined(_PLATFORM_WIN32)
  unsigned int gVBO;
  unsigned int gVAO;
  unsigned int gUVBO;
  unsigned int gNORMBO;
  unsigned int gCOLOURBO;
  unsigned int gIBO;
#elif defined(_PLATFORM_PS4)
  // Pointer to the vertex buffer in local memory
  // stVertex *vertexBuffer;
  unsigned int vertexBufferOffset;
  uint32_t kIndexCount;
  // std::vector<uint16_t> indices;
  uint16_t *localindexData;
  Vertex *localvertexData;

  sce::Gnm::Buffer buffers[kVertexElemCount];
  sce::Gnm::Texture *texture;
  uint16_t *indexData;
#endif
};