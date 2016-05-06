#define GLEW_STATIC
#include "GL/glew.h"
#include "pc_menu.h"
#include "pc_shaderprogram.h"
#include "resource.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

namespace Platform_Menu {
static GLuint gIBO = -1;

// 0 ---- 1
// |      |
// 2 ---- 3 -- 4
const uint16_t indicies_[] = {0, 2, 1, 2, 3, 1, 3, 4, 1};

struct RBO {
  GLuint gVAO;
  GLuint gVBO;
  // instanced
  GLuint gPBO;
  GLuint gCBO;
};

void *Prep() {
  auto r = new RBO();
  glGenVertexArrays(1, &r->gVAO);
  glGenBuffers(1, &r->gVBO);
  glGenBuffers(1, &r->gPBO);
  glGenBuffers(1, &r->gCBO);

  glBindVertexArray(r->gVAO);

  glBindBuffer(GL_ARRAY_BUFFER, r->gVBO);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), 0);

  glBindBuffer(GL_ARRAY_BUFFER, r->gPBO);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), 0);
  glVertexAttribDivisor(1, 1);

  glBindBuffer(GL_ARRAY_BUFFER, r->gCBO);
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 4, GL_FLOAT, GL_TRUE, sizeof(glm::vec4), 0);
  glVertexAttribDivisor(2, 1);

  glBindBuffer(GL_ARRAY_BUFFER, NULL);

  if (gIBO == -1) {
    glGenBuffers(1, &gIBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 9 * sizeof(uint16_t), &indicies_[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);
  }
  glBindVertexArray(NULL);

  return r;
}

void Update(void *deets, const glm::vec2 *verts, const vector<glm::vec2> &positions,
            const vector<glm::vec4> &colours) {

  auto r = static_cast<RBO *>(deets);
  glBindBuffer(GL_ARRAY_BUFFER, r->gVBO);
  glBufferData(GL_ARRAY_BUFFER, 5 * sizeof(glm::vec2), &verts[0], GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, r->gPBO);
  glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec2), &positions[0],
               GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, r->gCBO);
  glBufferData(GL_ARRAY_BUFFER, colours.size() * sizeof(glm::vec4), &colours[0], GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, NULL);
}

void Render(void *deets, size_t count) {
  auto r = static_cast<RBO *>(deets);

  // glEnable(GL_BLEND);
  // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glBindVertexArray(r->gVAO);
  const auto prog = Storage<ShaderProgram>::Get("2dmenu")->program;
  glUseProgram(prog);
  glm::mat4 proj = glm::ortho(0.0f, (float)DEFAULT_RESOLUTION_X, (float)DEFAULT_RESOLUTION_Y, 0.0f,
                              -100.0f, 100.0f);
  glUniformMatrix4fv(glGetUniformLocation(prog, "projection"), 1, 0, glm::value_ptr(proj));

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIBO);
  glDrawElementsInstanced(GL_TRIANGLES, 9, GL_UNSIGNED_SHORT, 0, (GLsizei)count);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);
  glBindVertexArray(NULL);
}
}