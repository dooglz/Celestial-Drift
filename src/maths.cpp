#include "maths.h"
#include <string>

/*
vec3 GetUpVector(const quat &q) {
  return vec3(q * vec4(0,1.0f,0,1.0f)); // TODO: I just worte this idk if worky
}
*/
vec3 GetUpVector(const quat &q) {
  return vec3(2 * (q.x * q.y - q.w * q.z), 1.0f - 2.0f * (q.x * q.x + q.z * q.z), 2.0f * (q.y * q.z + q.w * q.x));
}

vec3 GetRightVector(const quat &q) {
  return vec3(1.0f - 2.0f * (q.y * q.y + q.z * q.z), 2.0f * (q.x * q.y + q.w * q.z), 2.0f * (q.x * q.z - q.w * q.y));
}

vec3 GetForwardVector(const quat &q) {
  return vec3(2.0f * (q.x * q.z + q.w * q.y), 2.0f * (q.y * q.z - q.w * q.x), 1.0f - 2.0f * (q.x * q.x + q.y * q.y));
}

const std::string tos(const vec3 &v) {
  return "(" + std::to_string(v.x) + "," + std::to_string(v.y) + "," + std::to_string(v.z) + ")";
}