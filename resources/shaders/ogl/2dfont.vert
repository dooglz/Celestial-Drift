#version 430 core
uniform mat4 projection;

layout(location = 0) in vec3 vertexPos;
layout(location = 1) in vec2 vertexUv;
layout(location = 2) in vec4 vertexColor;

out vec4 fragCol;
out vec2 fragTexCoord;
void main()
{
	fragTexCoord = vertexUv;
	fragCol = vertexColor;
	gl_Position = projection * vec4(vertexPos.x, vertexPos.y, 0.0, 1.0);
}
