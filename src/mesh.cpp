#include "mesh.h"
#include "filesystem.h"
#include "maths.h"
#include "resource.h"
//#include "pc_shaderprogram.h"
#include "tiny_obj_loader.h"
#include <string>

using namespace std;

Mesh *ParseOBJFile(fileIO::Textfile *tf);

Mesh *Mesh::Load(const std::string &name) {
  const auto s = name.substr(name.length() - 3);
  if (s == "obj") {
    return (ParseOBJFile(Storage<fileIO::Textfile>::Get(name)));
  } else {
    LOG(logERROR) << "Unkown mesh file format";
    assert(false);
    exit(1);
  }
}

Mesh *ParseOBJFile(fileIO::Textfile *tf) {
  std::vector<vec3> vertices;
  std::vector<vec2> uvs;
  std::vector<vec3> normals;
  std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
  std::vector<vec3> temp_vertices;
  std::vector<vec2> temp_uvs;
  std::vector<vec3> temp_normals;

  std::vector<tinyobj::shape_t> shapes;
  std::vector<tinyobj::material_t> materials;
  string err;
  //tinyobj::LoadObj(shapes, materials, err, *tf, false);
  tinyobj::LoadObj(shapes,materials,err,*tf,"",false);
  assert(shapes.size() > 0);

  Mesh *pm = new Mesh();

  for (size_t i = 0; i < shapes.size(); i++) {
    Mesh *m;
    if (i > 0) {
      m = new Mesh();
      pm->subMeshes.push_back(m);
    } else {
      m = pm;
    }
    m->objGroupName = shapes[i].name;
    m->strip = false;
    m->fan = false;
    m->line = false;
    m->shaderPref = "basic";

    tinyobj::mesh_t *tm = &shapes[i].mesh;
    m->loadedLocal = false;
    m->loadedMain = true;
    m->numVerts = (uint32_t)tm->positions.size() / 3;

    m->vertexData.resize(m->numVerts, vec3(0.0f, 0.0f, 0.0f));
    memcpy(&m->vertexData[0], &tm->positions[0], sizeof(float) * tm->positions.size());

    if (tm->normals.size() > 0) {
      m->hasNormals = true;
      m->normals.resize(tm->normals.size() / 3, vec3(0.0f, 0.0f, 0.0f));
      memcpy(&m->normals[0], &tm->normals[0], sizeof(float) * tm->normals.size());
    } else {
      m->hasNormals = false;
    }

    if (tm->indices.size() > 0) {
      m->hasIndicies = true;
      m->indices = tm->indices;
    } else {
      m->hasIndicies = false;
    }

    if (tm->lines.size() > 0) {
      m->hasLines = true;
      m->lines = tm->lines;
    } else {
      m->hasLines = false;
    }

    if (tm->texcoords.size() > 0) {
      m->hasUvs = true;
      m->uvs.resize(tm->texcoords.size() / 2, vec2(0.0f, 0.0f));
      memcpy(&m->uvs[0], &tm->texcoords[0], sizeof(float) * tm->texcoords.size());

    } else {
      m->hasUvs = false;
    }

    unsigned int col[4] = {urandomColor(), urandomColor(), urandomColor()};
    for (size_t i = 0; i < m->numVerts; i++) {
      m->colours.push_back(col[i % 3]);
    }
    m->hasColours = true;
  }
  return pm;
}