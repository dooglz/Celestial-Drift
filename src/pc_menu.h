#include "common.h"
#include <glm/glm.hpp>
#include <vector>
using namespace std;

namespace Platform_Menu {
void *Prep();
void Update(void *deets, const glm::vec2 *verts, const vector<glm::vec2> &positions,
            const vector<glm::vec4> &colours);
void Render(void *deets, size_t count);
}