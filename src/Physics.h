#pragma once
#include <glm\glm.hpp>
#include "entity.h"
#include "component_shipdriver.h"

bool Collide(const glm::vec3 &sphereCenter, const float radius, const glm::vec3 *vertices,
             const glm::mat4 &mm, glm::vec3 &point, glm::vec3 &resultNormal, float &depth);

bool Collide(const glm::vec3 &sphereCenter, const float radius, const glm::vec3 *vertices,
             glm::vec3 &point, glm::vec3 &resultNormal, float &depth);

bool Collide(const glm::vec3 &sphereCenter, const float radius, const glm::vec3 &sphereCenter2,
             const float radius2, glm::vec3 &point, glm::vec3 &resultNormal, float &depth);

void Resolve(Entity &ent, Components::CmShipdriver &sd, vec3 &resultNormal, float &depth);