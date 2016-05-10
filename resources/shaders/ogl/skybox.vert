#version 430

in vec3 vp;
layout(location = 0)uniform mat4 PV;
out vec3 texcoords;

void main () {
  texcoords = vp;
  gl_Position = (PV * vec4 (vp, 1.0)).xyww;
}