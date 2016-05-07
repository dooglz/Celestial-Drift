#pragma once
#include "common.h"
#include <string>

namespace fileIO {/*
static const std::string filedirs[] = {
    "", "res/", "res/mdl/", "res/fonts/", "res/shaders/ogl/", "..\\", "..\\res/", "..\\res/mdl/",
    "..\\res/fonts/", "..\\res/shaders/ogl/", "/app0/", "/app0/res/", "/app0/res/mdl/",
    "/app0/res/fonts/", "/app0/res/shaders/ps4/"};
*/
static const std::string filedirs[] = {"", "resources/", "resources/mdl/", "resources/fonts/",
                                       "resources/shaders/ogl/"};

struct BinaryFile {
  static BinaryFile *Load(const std::string &name);
  size_t size;
  char *data;
  uint32_t *intdata;
};
struct Textfile {
  static Textfile *Load(const std::string &name);
  BinaryFile data;
  size_t lines;
  uint32_t *linelengths;
  char *GetLinePtr(size_t line);
  std::string GetLine(size_t line);
};

// Textfile* LoadTextFile(const std::string& name);
// BinaryFile* LoadBinaryFile(const std::string& name);

void UnloadTextFile(Textfile *);
void UnloadBinaryFile(BinaryFile *);
bool FileExists(const std::string &name);
const std::string SearchDirsPath(const std::string &name);
}