#include "GL/glew.h"
#include <string>

enum SHADER_TYPE { V_F, V_G_F, V_G_T_F };

class ShaderProgram {
public:
  ShaderProgram();
  ~ShaderProgram();
  static ShaderProgram *Load(const std::string &name);
  GLuint program;

protected:
};
