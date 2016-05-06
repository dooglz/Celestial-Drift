#include "common.h"
#include "component_camera.h"
#include "glm/glm.hpp"
#include "mesh.h"
#include "pc_renderer.h"
#include "pc_shaderprogram.h"
#include "renderer.h"
#include "resource.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

glm::mat4 PC_Renderer::vp_;
std::vector<std::pair<const glm::vec3, const glm::vec4>> PC_Renderer::linebuffer;

void CheckGL() {
  GLenum err;
  while ((err = glGetError()) != GL_NO_ERROR) {
    printf("An OGL error has occured: %u\n", err);
  }
}

void PC_Renderer::PostRender() { ProcessLines(); };

bool PC_Renderer::Init() {
  // preload basic shaders
  Storage<ShaderProgram>::Get("basic");
  glViewport(0, 0, DEFAULT_RESOLUTION);
  glDepthFunc(GL_LESS);
  glEnable(GL_DEPTH_TEST);
  return false;
}

bool PC_Renderer::Shutdown() { return false; }

void PC_Renderer::DrawCross(const glm::vec3 &p1, const float size, const glm::vec4 &col) {
  DrawLine(p1 + glm::vec3(size, 0, 0), p1 - glm::vec3(size, 0, 0), col, col);
  DrawLine(p1 + glm::vec3(0, size, 0), p1 - glm::vec3(0, size, 0), col, col);
  DrawLine(p1 + glm::vec3(0, 0, size), p1 - glm::vec3(0, 0, size), col, col);
}

void PC_Renderer::DrawLine(const glm::vec3 &p1, const glm::vec3 &p2, const glm::vec4 &col1,
                           const glm::vec4 &col2) {
  linebuffer.push_back(std::make_pair(p1, col1));
  linebuffer.push_back(std::make_pair(p2, col2));
}

void PC_Renderer::SetViewMatrix(const glm::mat4 &vpm) {
  glm::mat4 Projection =
      glm::perspective(glm::radians(45.0f),
                       (float)DEFAULT_RESOLUTION_X / (float)DEFAULT_RESOLUTION_Y, 0.1f, 1000.0f);
  vp_ = Projection * vpm;
}

void PC_Renderer::RenderMesh(const Mesh &m, const mat4 &modelMatrix) {
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
  CheckGL();

  std::string shadername = "basic";
  if (m.shaderPref != "") {
    shadername = m.shaderPref;
    if (Storage<ShaderProgram>::Get(shadername) == nullptr) {
      LOG(logWARNING) << "Can't find shader " << shadername << "for model";
      shadername = "basic";
    }
  }

  const auto prog = Storage<ShaderProgram>::Get(shadername)->program;
  glUseProgram(prog);
  CheckGL();

  GLint vpIn = glGetUniformLocation(prog, "viewprojection");
  GLint mvpIn = glGetUniformLocation(prog, "modelprojection");
  GLint invmvpIn = glGetUniformLocation(prog, "invMvp");
  CheckGL();

  glUniformMatrix4fv(vpIn, 1, false, glm::value_ptr(vp_));
  glUniformMatrix4fv(mvpIn, 1, false, glm::value_ptr(modelMatrix));
  if (invmvpIn != -1) {
    mat4 inv = inverse(vp_ * modelMatrix);
    glUniformMatrix4fv(invmvpIn, 1, false, glm::value_ptr(inv));
  }
  CheckGL();

  // Bind to VAO
  glBindVertexArray(m.gVAO);
  glBindBuffer(GL_ARRAY_BUFFER, m.gVBO);
  if (m.hasIndicies) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m.gIBO);
  }
  // DRAW
  if (m.line == true) {
    if (m.strip == true) {
      if (m.hasIndicies) {
        glDrawElements(GL_LINE_STRIP, (GLsizei)m.indices.size(), GL_UNSIGNED_INT, (void *)0);
      } else {
        glDrawArrays(GL_LINE_STRIP, 0, m.numVerts);
      }
    } else {
      if (m.hasIndicies) {
        glDrawElements(GL_LINES, (GLsizei)m.indices.size(), GL_UNSIGNED_INT, (void *)0);
      } else {
        glDrawArrays(GL_LINES, 0, m.numVerts);
      }
    }
  } else {
    if (m.fan == true) {
      if (m.hasIndicies) {
        glDrawElements(GL_TRIANGLE_FAN, (GLsizei)m.indices.size(), GL_UNSIGNED_INT, (void *)0);
      } else {
        glDrawArrays(GL_TRIANGLE_FAN, 0, m.numVerts);
      }
    } else if (m.strip == true) {
      if (m.hasIndicies) {
        glDrawElements(GL_TRIANGLE_STRIP, (GLsizei)m.indices.size(), GL_UNSIGNED_INT, (void *)0);
      } else {
        glDrawArrays(GL_TRIANGLE_STRIP, 0, m.numVerts);
      }
    } else {
      if (m.hasIndicies) {
        glDrawElements(GL_TRIANGLES, (GLsizei)m.indices.size(), GL_UNSIGNED_INT, (void *)0);
      } else {
        glDrawArrays(GL_TRIANGLES, 0, m.numVerts);
      }
    }
  }

  glBindBuffer(GL_ARRAY_BUFFER, NULL);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);
  glBindVertexArray(0);
  glUseProgram(NULL);
}

void PC_Renderer::LoadMesh(Mesh *msh) {
  if (msh->loadedLocal) {
    return;
  }
  assert(msh->loadedMain);

  // Generate VAO
  glGenVertexArrays(1, &msh->gVAO);

  // Bind VAO
  glBindVertexArray(msh->gVAO);

  { // Vertex
    // Generate VBO
    glGenBuffers(1, &(msh->gVBO));

    // Bind VBO
    glBindBuffer(GL_ARRAY_BUFFER, (msh->gVBO));
    // put the data in it
    glBufferData(GL_ARRAY_BUFFER, msh->vertexData.size() * sizeof(vec3), &msh->vertexData[0],
                 GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);

    glVertexAttribPointer(0,            // index
                          3,            // size
                          GL_FLOAT,     // type
                          GL_FALSE,     // normalised
                          sizeof(vec3), // stride
                          NULL          // pointer/offset
                          );
    CheckGL();
  }

  if (msh->hasColours) { // colours data
    // Generate VBO
    glGenBuffers(1, &(msh->gCOLOURBO));
    // Bind VBO
    glBindBuffer(GL_ARRAY_BUFFER, (msh->gCOLOURBO));
    // put the data in it
    glBufferData(GL_ARRAY_BUFFER, msh->colours.size() * sizeof(unsigned int), &msh->colours[0],
                 GL_STATIC_DRAW);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,                    // index
                          4,                    // size
                          GL_UNSIGNED_BYTE,     // type
                          GL_TRUE,              // normalised
                          sizeof(unsigned int), // stride
                          NULL                  // pointer/offset
                          );
    glBindBuffer(GL_ARRAY_BUFFER, NULL);
    CheckGL();
  }

  if (msh->hasUvs) {

    // Generate BO
    glGenBuffers(1, &(msh->gUVBO));
    // Bind VBO
    glBindBuffer(GL_ARRAY_BUFFER, (msh->gUVBO));

    // put the data in it
    glBufferData(GL_ARRAY_BUFFER, msh->uvs.size() * sizeof(vec2), &msh->uvs[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2,            // index
                          2,            // size
                          GL_FLOAT,     // type
                          GL_FALSE,     // normalised
                          sizeof(vec2), // stride
                          NULL          // pointer/offset
                          );
    glBindBuffer(GL_ARRAY_BUFFER, NULL);
    CheckGL();
  }

  if (msh->hasIndicies) {
    glGenBuffers(1, &(msh->gIBO));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, msh->gIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, msh->indices.size() * sizeof(uint32_t), &msh->indices[0],
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);
    CheckGL();
  }

  // Unblind VAO
  glBindVertexArray(NULL);
  CheckGL();
  msh->loadedLocal = true;
}

void PC_Renderer::ClearColour(glm::vec4 c) {
  glClearColor(c.x, c.y, c.z, c.w);
  glClearDepth(1.0);
}

void PC_Renderer::ClearFrame() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void PC_Renderer::ProcessLines() {
  if (linebuffer.size() < 1) {
    return;
  }
  // Generate VAO
  unsigned int vao;
  glGenVertexArrays(1, &vao);
  CheckGL();

  // Bind VAO
  glBindVertexArray(vao);
  CheckGL();

  // Generate VBO
  unsigned int vbo;
  glGenBuffers(1, &vbo);
  CheckGL();

  // Bind VBO
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  CheckGL();

  // put the data in it
  glBufferData(GL_ARRAY_BUFFER, linebuffer.size() * sizeof(float) * 7, &linebuffer[0],
               GL_STATIC_DRAW);
  CheckGL();

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0,                 // index
                        3,                 // size
                        GL_FLOAT,          // type
                        GL_FALSE,          // normalised
                        sizeof(float) * 7, // stride
                        NULL               // pointer/offset
                        );
  CheckGL();

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1,                            // index
                        4,                            // size
                        GL_FLOAT,                     // type
                        GL_TRUE,                      // normalised
                        sizeof(float) * 7,            // stride
                        (GLvoid *)(sizeof(float) * 3) // pointer/offset
                        );
  CheckGL();

  if (Storage<ShaderProgram>::Get("basic") == nullptr) {
    LOG(logWARNING) << "Can't find shader basic for lines";
  }

  const auto prog = Storage<ShaderProgram>::Get("basic")->program;
  glUseProgram(prog);
  CheckGL();

  // get shader input indexes
  GLint vpIn = glGetUniformLocation(prog, "viewprojection");
  GLint mvpIn = glGetUniformLocation(prog, "modelprojection");
  CheckGL();

  // Send MVP
  glUniformMatrix4fv(vpIn, 1, false, glm::value_ptr(vp_));
  glUniformMatrix4fv(mvpIn, 1, false, glm::value_ptr(glm::mat4(1.0f)));
  CheckGL();

  glDisable(GL_DEPTH_TEST);
  glDrawArrays(GL_LINES, 0, linebuffer.size());
  CheckGL();

  glDeleteBuffers(1, &vbo);
  glDeleteVertexArrays(1, &vao);
  glBindVertexArray(NULL);
  CheckGL();
  linebuffer.clear();
}