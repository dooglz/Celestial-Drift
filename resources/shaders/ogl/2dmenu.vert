#version 430 core
uniform mat4 projection;

layout(location = 0) in vec2 vertexPos;
layout(location = 1) in vec2 vertexOffset;
layout(location = 2) in vec4 vertexColor;

out vec4 fragCol;
void main() {
  fragCol = vertexColor;
  vec2 pos = vertexPos + vertexOffset;
  gl_Position = projection * vec4(pos.x, pos.y, -1.0, 1.0);
}
