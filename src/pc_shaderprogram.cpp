#include "pc_shaderprogram.h"
#include "filesystem.h"
#include "resource.h"

ShaderProgram::ShaderProgram() {}

ShaderProgram::~ShaderProgram() {}

ShaderProgram *ShaderProgram::Load(const std::string &name) {
  fileIO::Textfile *vp_raw = nullptr;
  fileIO::Textfile *fp_raw = nullptr;
  fileIO::Textfile *gp_raw = nullptr;
  fileIO::Textfile *tp_raw = nullptr;

  if (fileIO::FileExists(name + ".vert") && fileIO::FileExists(name + ".frag")) {
    vp_raw = Storage<fileIO::Textfile>::Get(name + ".vert");
    fp_raw = Storage<fileIO::Textfile>::Get(name + ".frag");
    if (fileIO::FileExists(name + ".geom")) {
      gp_raw = Storage<fileIO::Textfile>::Get(name + ".geom");
    }
    if (fileIO::FileExists(name + ".tess")) {
      tp_raw = Storage<fileIO::Textfile>::Get(name + ".tess");
    }
  } else {
    LOG(logERROR) << "Couldn't load shader: " << name;
    assert(false);
  }

  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  const GLchar *vssource = (const GLchar *)vp_raw->data.data;
  glShaderSource(vertexShader, 1, &vssource, (GLint *)&(vp_raw->data.size));

  // Compile the vertex shader
  glCompileShader(vertexShader);

  GLint isCompiled = 0;
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
  if (isCompiled == GL_FALSE) {
    GLint maxLength = 0;
    glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

    // The maxLength includes the NULL character
    std::string infoLog(maxLength, 0);
    glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

    // We don't need the shader anymore.
    glDeleteShader(vertexShader);

    // Use the infoLog as you see fit.
    LOG(logERROR) << name << "  - Vert : " <<infoLog << std::endl;

    // In this simple program, we'll just leave
    return nullptr;
  }

  // Create an empty fragment shader handle
  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

  // Send the fragment shader source code to GL
  // Note that std::string's .c_str is NULL character terminated.
  const GLchar *fssource = (const GLchar *)fp_raw->data.data;
  glShaderSource(fragmentShader, 1, &fssource, (GLint *)&(fp_raw->data.size));

  // Compile the fragment shader
  glCompileShader(fragmentShader);

  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
  if (isCompiled == GL_FALSE) {
    GLint maxLength = 0;
    glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

    // The maxLength includes the NULL character
    std::string infoLog(maxLength, 0);
    glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

    // We don't need the shader anymore.
    glDeleteShader(fragmentShader);
    // Either of them. Don't leak shaders.
    glDeleteShader(vertexShader);

    // Use the infoLog as you see fit.
    LOG(logERROR) << name << "  - frag : " <<infoLog << std::endl;

    return nullptr;
  }

  // Vertex and fragment shaders are successfully compiled.
  // Now time to link them together into a program.
  // Get a program object.
  GLuint program = glCreateProgram();

  // Attach our shaders to our program
  glAttachShader(program, vertexShader);
  glAttachShader(program, fragmentShader);

  // Link our program
  glLinkProgram(program);

  // Note the different functions here: glGetProgram* instead of glGetShader*.
  GLint isLinked = 0;
  glGetProgramiv(program, GL_LINK_STATUS, (int *)&isLinked);
  if (isLinked == GL_FALSE) {
    GLint maxLength = 0;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

    // The maxLength includes the NULL character
    std::vector<GLchar> infoLog(maxLength);
    glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

    // We don't need the program anymore.
    glDeleteProgram(program);
    // Don't leak shaders either.
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Use the infoLog as you see fit.

    // In this simple program, we'll just leave
    return nullptr;
  }

  // Always detach shaders after a successful link.
  glDetachShader(program, vertexShader);
  glDetachShader(program, fragmentShader);

  // TODO: UNLOAD SHIT
  ShaderProgram *sp = new ShaderProgram();
  sp->program = program;
  return sp;
}
