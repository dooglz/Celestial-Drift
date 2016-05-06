#include "Physics.h"
#include "entity.h"
#include <glm\gtx\norm.hpp>
#include "component_shipdriver.h"

using namespace glm;
#define SIMD_EPSILON 1.1920928955078125E-7f

bool pointInTriangle(const vec3 vertices[], const vec3 &normal, vec3 *p) {
  const vec3 *p1 = &vertices[0];
  const vec3 *p2 = &vertices[1];
  const vec3 *p3 = &vertices[2];

  vec3 edge1(*p2 - *p1);
  vec3 edge2(*p3 - *p2);
  vec3 edge3(*p1 - *p3);

  vec3 p1_to_p(*p - *p1);
  vec3 p2_to_p(*p - *p2);
  vec3 p3_to_p(*p - *p3);

  vec3 edge1_normal(cross(edge1, normal));
  vec3 edge2_normal(cross(edge2, normal));
  vec3 edge3_normal(cross(edge3, normal));

  float r1, r2, r3;
  r1 = dot(edge1_normal, p1_to_p);
  r2 = dot(edge2_normal, p2_to_p);
  r3 = dot(edge3_normal, p3_to_p);
  if ((r1 > 0 && r2 > 0 && r3 > 0) || (r1 <= 0 && r2 <= 0 && r3 <= 0)) {
    return true;
  }
  return false;
}

bool facecontains(const vec3 &p, const vec3 *vertices, vec3 &normal) {
  vec3 lp(p);
  vec3 lnormal(normal);
  return pointInTriangle(vertices, lnormal, &lp);
}

float SegmentSqrDistance(const vec3 &from, const vec3 &to, const vec3 &p, vec3 &nearest) {
  vec3 diff = p - from;
  vec3 v = to - from;
  float t = dot(v, diff);

  if (t > 0) {
    float dotVV = dot(v, v);
    if (t < dotVV) {
      t /= dotVV;
      diff -= t * v;

    } else {
      t = 1;
      diff -= v;
    }

  } else
    t = 0;

  nearest = from + t * v;
  return dot(diff, diff);
}

bool Collide(const vec3 &sphereCenter, const float radius, const vec3 *vertices, const mat4 &mm,
             vec3 &point, vec3 &resultNormal, float &depth, bool stationary) {

  vec3 normal = cross((vertices[1] - vertices[0]), (vertices[2] - vertices[0]));
  normal = normalize(normal);
  if (length2(normal) == 0) {
    return false;
  }
  vec3 trnvertices[3];
  if (stationary) {
    trnvertices[0] = vertices[0];
    trnvertices[1] = vertices[1];
    trnvertices[2] = vertices[2];
  } else {
    trnvertices[0] = vec3(mm * vec4(vertices[0], 1));
  }
  vec3 p1ToCentre = sphereCenter - trnvertices[0];
  float distanceFromPlane = dot(p1ToCentre, normal);

  if (distanceFromPlane < 0.0f) {
    // triangle facing the other way
    distanceFromPlane *= -1.0f;
    normal *= -1.0f;
  }

  bool isInsideContactPlane = distanceFromPlane < radius;

  // Check for contact / intersection
  bool hasContact = false;
  vec3 contactPoint;
  if (isInsideContactPlane) {
    if (!stationary) {
      trnvertices[1] = vec3(mm * vec4(vertices[1], 1));
      trnvertices[2] = vec3(mm * vec4(vertices[2], 1));
    }
    if (facecontains(sphereCenter, trnvertices, normal)) {
      // Inside the contact wedge - touches a point on the shell plane
      hasContact = true;
      contactPoint = sphereCenter - normal * distanceFromPlane;

    } else {
      // Could be inside one of the contact capsules
      float contactCapsuleRadiusSqr = radius * radius;
      vec3 nearestOnEdge;

      for (int i = 0; i < 3; i++) {

        vec3 pa;
        vec3 pb;

        int index0 = i % 3;
        int index1 = (i + 1) % 3;
        pa = trnvertices[index0];
        pb = trnvertices[index1];

        float distanceSqr = SegmentSqrDistance(pa, pb, sphereCenter, nearestOnEdge);
        if (distanceSqr < contactCapsuleRadiusSqr) {
          // Yep, we're inside a capsule
          hasContact = true;
          contactPoint = nearestOnEdge;
        }
      }
    }
  }

  if (hasContact) {
    vec3 contactToCentre = sphereCenter - contactPoint;
    float distanceSqr = length2(contactToCentre);

    if (distanceSqr < radius * radius) {
      if (distanceSqr > SIMD_EPSILON) {
        float distance = sqrt(distanceSqr);
        resultNormal = contactToCentre;
        resultNormal = normalize(resultNormal);
        point = contactPoint;
        depth = -(radius - distance);
      } else {
        resultNormal = normal;
        point = contactPoint;
        depth = -radius;
      }
      return true;
    }
  }
  return false;
}

bool Collide(const vec3 &sphereCenter, const float radius, const vec3 *vertices, const mat4 &mm,
             vec3 &point, vec3 &resultNormal, float &depth) {
  return Collide(sphereCenter, radius, vertices, mm, point, resultNormal, depth, false);
}

bool Collide(const vec3 &sphereCenter, const float radius, const vec3 *vertices, vec3 &point,
             vec3 &resultNormal, float &depth) {
  const mat4 a;
  return Collide(sphereCenter, radius, vertices, a, point, resultNormal, depth, true);
}

void Resolve(Entity& ent, Components::CmShipdriver& sd, vec3& resultNormal, float& depth){
  // LOG(logERROR) << depth << " " << i << " " << tos(point) << " " <<tos(resultNormal);
  /*
  Renderer::DrawCross(p[0], 5.0f, vec4(1.0f, 0.5f, 1.0f, 1.0f));
  Renderer::DrawCross(p[1], 5.0f, vec4(1.0f, 0.5f, 1.0f, 1.0f));
  Renderer::DrawCross(p[2], 5.0f, vec4(1.0f, 0.5f, 1.0f, 1.0f));
  Renderer::DrawCross(point, 5.0f, vec4(0.5f, 0.5f, 1.0f, 1.0f));
  Renderer::DrawLine(p[0], p[1], vec4(1.0f, 0.5f, 1.0f, 1.0f), vec4(1.0f, 0.5f, 1.0f, 1.0f));
  Renderer::DrawLine(p[0], p[2], vec4(1.0f, 0.5f, 1.0f, 1.0f), vec4(1.0f, 0.5f, 1.0f, 1.0f));
  Renderer::DrawLine(p[2], p[1], vec4(1.0f, 0.5f, 1.0f, 1.0f), vec4(1.0f, 0.5f, 1.0f, 1.0f));
  */
  const float coef = 0.1f;

  // Calculate relative velocity in terms of the normal direction
  float velAlongNormal = glm::dot(sd.velocity_, resultNormal);
  // Do not resolve if velocities are separating
  if (velAlongNormal > 0) {
    return;
  }
  // Calculate impulse scalar
  float j = -(1 + coef) * velAlongNormal;

  sd.velocity_ += (resultNormal * j);
  // correction for sink
  const float percent = 0.8; // usually 20% to 80%
  const float slop = 0.001;  // usually 0.01 to 0.1
  vec3 correction = max((-depth) - slop, 0.0f) * percent * resultNormal;
  ent.SetPosition(ent.GetPosition() + correction);
}