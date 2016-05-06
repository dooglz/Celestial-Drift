#pragma once

#include <map>
#include <string>
#include <vector>
#include "filesystem.h"

namespace tinyobj {

struct material_t {
  std::string name;

  float ambient[3];
  float diffuse[3];
  float specular[3];
  float transmittance[3];
  float emission[3];
  float shininess;
  float ior;      // index of refraction
  float dissolve; // 1 == opaque; 0 == fully transparent
  // illumination model (see http://www.fileformat.info/format/material/)
  int illum;

  int dummy; // Suppress padding warning.

  std::string ambient_texname;            // map_Ka
  std::string diffuse_texname;            // map_Kd
  std::string specular_texname;           // map_Ks
  std::string specular_highlight_texname; // map_Ns
  std::string bump_texname;               // map_bump, bump
  std::string displacement_texname;       // disp
  std::string alpha_texname;              // map_d
  std::map<std::string, std::string> unknown_parameter;
};

struct tag_t {
  std::string name;

  std::vector<int> intValues;
  std::vector<float> floatValues;
  std::vector<std::string> stringValues;
};

struct mesh_t {
  std::vector<float> positions;
  std::vector<float> normals;
  std::vector<float> texcoords;
  std::vector<unsigned int> indices;
  std::vector<unsigned int> lines;
  std::vector<unsigned char> num_vertices; // The number of vertices per face. Up to 255.
  std::vector<int> material_ids;           // per-face material ID
  std::vector<tag_t> tags;                 // SubD tag
};

struct shape_t {
  std::string name;
  mesh_t mesh;
};

class MaterialReader {
public:
  MaterialReader() {}
  virtual ~MaterialReader();

  virtual bool operator()(const std::string &matId, std::vector<material_t> &materials,
                          std::map<std::string, int> &matMap, std::string &err) = 0;
};

class MaterialFileReader : public MaterialReader {
public:
  MaterialFileReader(const std::string &mtl_basepath) : m_mtlBasePath(mtl_basepath) {}
  virtual ~MaterialFileReader() {}
  virtual bool operator()(const std::string &matId, std::vector<material_t> &materials,
                          std::map<std::string, int> &matMap, std::string &err);

private:
  std::string m_mtlBasePath;
};

// Loads .obj from a file.
// 'shapes' will be filled with parsed shape data
// The function returns error string.
// Returns true when loading .obj become success.
// Returns warning and error message into `err`
// 'mtl_basepath' is optional, and used for base path for .mtl file.
// 'triangulate' is optional, and used whether triangulate polygon face in .obj
// or not.
bool LoadObj(std::vector<shape_t> &shapes,       // [output]
             std::vector<material_t> &materials, // [output]
             std::string &err,                   // [output]
             const char *filename, const char *mtl_basepath = NULL, bool triangulate = true);

bool LoadObj(std::vector<shape_t> &shapes,       // [output]
             std::vector<material_t> &materials, // [output]
             std::string &err,                   // [output]
             fileIO::Textfile tf, const char *mtl_basepath = NULL, bool triangulate = true);

/// Loads object from a std::istream, uses GetMtlIStreamFn to retrieve
/// std::istream for materials.
/// Returns true when loading .obj become success.
/// Returns warning and error message into `err`
bool LoadObj(std::vector<shape_t> &shapes,       // [output]
             std::vector<material_t> &materials, // [output]
             std::string &err,                   // [output]
             std::istream &inStream, MaterialReader &readMatFn, bool triangulate = true);

bool LoadObj(std::vector<shape_t> &shapes,       // [output]
             std::vector<material_t> &materials, // [output]
             std::string &err,                   // [output]
             fileIO::Textfile tf, MaterialReader &readMatFn, bool triangulate = true);

/// Loads materials into std::map
void LoadMtl(std::map<std::string, int> &material_map, // [output]
             std::vector<material_t> &materials,       // [output]
             std::istream &inStream);
}
