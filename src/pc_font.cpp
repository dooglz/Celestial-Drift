#include "pc_font.h"
#include "common.h"
#include "filesystem.h"
#include "freetype-gl/freetype-gl.h"
#include "pc_shaderprogram.h"
#include "resource.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <sstream>
#include <string>
#include <vector>

static void CheckGL() {
  GLenum err;
  while ((err = glGetError()) != GL_NO_ERROR) {
    printf("An OGL error has occured: %u\n", err);
  }
}

const char *alphanum = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ!\"#$%&\'()*+"
                       ",-./:;?@[\\]^_`{|}~ ";

typedef struct {
  float x, y, z;    // position
  float s, t;       // texture
  float r, g, b, a; // color
} vertex_t;

GLuint gVAO;

GLuint gVBO;
GLuint gCBO;
GLuint gUBO;
GLuint gIBO;
std::vector<vertex_t> vb;
std::vector<GLuint> ib;
std::string prevFrameString;
std::stringstream frameString;

texture_font_t *fonts[11];
static const uint16_t fontsizes[] = {5, 10, 20, 30, 40, 50, 60, 80, 100, 160, 250};

void PC_Font::Clear() {
  vb.clear();
  ib.clear();
}

static texture_font_t *GetGoodSizedFont(const uint16_t req) {
  size_t i = 0;
  for (const auto s : fontsizes) {
    if (s > req) {
      if (i == 0) {
        return fonts[0];
      }
      const uint16_t lowerdif = req - fontsizes[i - 1];
      const uint16_t higerdif = s - req;
      if (lowerdif < higerdif) {
        return fonts[i - 1];
      } else {
        return fonts[i];
      }
    }
    ++i;
  }
  return fonts[i - 1];
}

void add_text(texture_font_t *font, const char *text, const glm::vec4 &color, const glm::vec2 &pos) {

  vec2 pen = {pos.x, pos.y};
  const auto originalx = pen.x;
  size_t i;
  float r = color.x, g = color.y, b = color.z, a = color.w;
  for (i = 0; i < strlen(text); ++i) {
    if (*(text + i) == '\n') {
      pen.y += font->height;
      pen.x = originalx;
      continue;
    }
    texture_glyph_t *glyph = texture_font_get_glyph(font, text + i);
    if (glyph != NULL) {
      float kerning = 0.0f;
      if (i > 0) {
        kerning = texture_glyph_get_kerning(glyph, text + i - 1);
      }
      pen.x += kerning;
      // X0Y0 ---- X1Y0
      //  |        2 |
      //  |  1       |
      //  X0Y1---- X1Y1
      float x0 = (float)(pen.x + glyph->offset_x);
      float x1 = (float)(x0 + glyph->width);
      float y0 = (float)(pen.y - glyph->offset_y);
      float y1;
      if (glyph->offset_y < glyph->height) {
        y1 = (float)(pen.y + (glyph->height - glyph->offset_y));
      } else {
        y1 = (float)(pen.y - (glyph->offset_y - glyph->height));
      }
      float s0 = glyph->s0;
      float t0 = glyph->t0;
      float s1 = glyph->s1;
      float t1 = glyph->t1;
      GLuint indices[6] = {0, 1, 2, 0, 2, 3};
      vertex_t vertices[4] = {{x0, y0, 0.0f, s0, t0, r, g, b, a},
                              {x0, y1, 0.0f, s0, t1, r, g, b, a},
                              {x1, y1, 0.0f, s1, t1, r, g, b, a},
                              {x1, y0, 0.0f, s1, t0, r, g, b, a}};
      const uint32_t current = (uint32_t)vb.size();
      for (auto &v : vertices) {
        vb.push_back(v);
      }
      for (auto &in : indices) {
        ib.push_back(in + current);
      }
      pen.x += glyph->advance_x;
    }
  }
}

void PC_Font::Draw(const uint16_t &fontsizes, const char *text, const glm::vec2 &position, const glm::vec4 &color) {

  add_text(GetGoodSizedFont(fontsizes), text, color, position);
  frameString << text << position.x << position.y << color.x << color.y << color.z;
}

texture_atlas_t *atlas;
bool PC_Font::Init() {
  atlas = texture_atlas_new(2048, 2048, 1);
  char *text = "A Quick Brown Fox Jumps Over The Lazy Dog 0123456789+=_-";
  std::string aa = fileIO::SearchDirsPath("consolas.ttf");
  const char *filename = aa.c_str();
  vec2 pen = {{5, 400}};
  vec4 black = {{0, 0, 0, 1}};
  for (const auto s : fontsizes) {
    static size_t i = 0;
    fonts[i] = texture_font_new_from_file(atlas, s, filename);
    texture_font_load_glyphs(fonts[i], alphanum);
    texture_atlas_upload(fonts[i]->atlas);
    ++i;
  }

  texture_font_t *font = fonts[1];

  glGenVertexArrays(1, &gVBO);
  glBindVertexArray(gVAO);
  glGenBuffers(1, &gVBO);
  glBindBuffer(GL_ARRAY_BUFFER, gVBO);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t), 0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (GLvoid *)(3 * sizeof(float)));
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 4, GL_FLOAT, GL_TRUE, sizeof(vertex_t), (GLvoid *)(5 * sizeof(float)));
  glBindBuffer(GL_ARRAY_BUFFER, NULL);
  glGenBuffers(1, &gIBO);
  glBindVertexArray(NULL);

  Storage<ShaderProgram>::Get("2dfont");
  CheckGL();
  return false;
}

bool PC_Font::Shutdown() {
  vb.clear();
  ib.clear();
  frameString.str("");
  prevFrameString = "";
  for (auto &f : fonts) {
    texture_font_delete(f);
    f = nullptr;
  }
  glDeleteBuffers(1, &gVBO);
  glDeleteBuffers(1, &gIBO);
  glDeleteBuffers(1, &gVAO);
  gVBO = -1;
  gIBO = -1;
  gVAO = -1;
  // TODO: delete shader
  return true;
}

void PC_Font::Render() {

  const std::string thisframestring = frameString.str();
  const bool newstuff = thisframestring.compare(prevFrameString);
  prevFrameString = thisframestring;
  frameString.str("");

  glBindVertexArray(gVAO);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  const auto prog = Storage<ShaderProgram>::Get("2dfont")->program;
  glUseProgram(prog);
  CheckGL();

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, atlas->id);
  CheckGL();

  const auto t = glGetUniformLocation(prog, "texture");
  if (t != -1) {
    glUniform1i(t, 0);
  }

  glm::mat4 proj = glm::ortho(0.0f, (float)DEFAULT_RESOLUTION_X, (float)DEFAULT_RESOLUTION_Y, 0.0f, -100.0f, 100.0f);
  glUniformMatrix4fv(glGetUniformLocation(prog, "projection"), 1, 0, glm::value_ptr(proj));

  glBindVertexArray(gVAO);
  glBindBuffer(GL_ARRAY_BUFFER, gVBO);
  if (newstuff) {
    glBufferData(GL_ARRAY_BUFFER, vb.size() * sizeof(vertex_t), &vb[0], GL_STATIC_DRAW);
  }
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIBO);
  if (newstuff) {
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, ib.size() * sizeof(GLuint), &ib[0], GL_STATIC_DRAW);
  }
  CheckGL();
  glDrawElements(GL_TRIANGLES, (GLsizei)ib.size(), GL_UNSIGNED_INT, (void *)0);
  CheckGL();

  glBindVertexArray(NULL);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);
  glBindBuffer(GL_ARRAY_BUFFER, NULL);

  // pastFrameString
  Clear();
}
