#version 330 core

in vec3 invpos;
// Ouput data
out vec4 color;

void main() {

  // Output color = color specified in the vertex shader,
  // interpolated between all 3 surrounding vertices
  // color = fragmentColor;
  //color = vec4(gl_FragCoord, 0.7f);
   float red = int(invpos.x * 4.0f) % 2;
   float blue = int(invpos.z * 4.0f) % 2;
  color = vec4(red,0.0f,blue, 1.0f -((gl_FragCoord.z / gl_FragCoord.w) / 30.0f));
   // color = vec4(0.0f,0.0f,1.0f, 1.0f);
 //color = vec4(invpos, 1.0f);

}
