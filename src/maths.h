#pragma once
#include "common.h"
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
using namespace glm;

/*#if defined(_MATH_LIB_GLM)
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
//using namespace glm;
//#define Quaternion glm::quat
#elif defined(_MATH_LIB_DXM)

#elif defined(_MATH_LIB_PS4)
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
//using namespace glm;
#endif
*/

inline int randomColor() {
  int x = rand() & 0xff;
  x |= (rand() & 0xff) << 8;
  x |= (rand() & 0xff) << 16;
  x |= (0xff) << 24;

  return x;
}
inline unsigned int urandomColor() {
  return static_cast<unsigned int>(randomColor());
}

vec3 GetForwardVector(const quat &q);
vec3 GetUpVector(const quat &q);
vec3 GetRightVector(const quat &q);

const std::string tos(const vec3 &v);

static inline float Frand() { return static_cast<float>(rand()) / static_cast<float>(RAND_MAX); }
