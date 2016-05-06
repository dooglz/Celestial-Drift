#include "maths.h"
#include <string>

vec3 GetForwardVector(const quat &q) {
  return vec3(2.0f * (q.x * q.z + q.w * q.y), 2.0f * (q.y * q.z - q.w * q.x),
              1.0f - 2.0f * (q.x * q.x + q.y * q.y));
}

const std::string tos(const vec3 &v) {
  return "(" + std::to_string(v.x) + "," + std::to_string(v.y) + "," + std::to_string(v.z) + ")";
}