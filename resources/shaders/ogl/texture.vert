#version 410 core

layout(location = 0) in vec3 LVertexPos2D;
layout(location = 1) in vec4 vertexColor;
layout(location = 2) in vec2 vertexUv;
uniform mat4 viewprojection;
uniform mat4 modelprojection;

out vec4 fragCol;
out vec2 fragTexCoord;
void main()
{
	fragTexCoord = vertexUv;
	fragCol = vertexColor;
	gl_Position = viewprojection * modelprojection * vec4(LVertexPos2D, 1);
}
