#version 410 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 LVertexPos2D;

// Values that stay constant for the whole mesh.
 out vec3 invpos;
// uniform mat4 invMvp;
uniform mat4 viewprojection;
uniform mat4 modelprojection;



void main() {
  gl_Position = viewprojection * modelprojection * vec4(LVertexPos2D, 1);

invpos = (modelprojection * vec4(LVertexPos2D, 1)).xyz;

  // invpos = (invMvp * gl_Position).xyz;
  // gl_Position = vec4(LVertexPos2D,1);
}
