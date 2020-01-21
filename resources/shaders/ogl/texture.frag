#version 410 core

uniform sampler2D texture0;

in vec4 fragCol;
in vec2 fragTexCoord;
out vec4 outColor;

void main()
{
   outColor = texture2D(texture0, fragTexCoord);
   outColor.r = fragTexCoord.x;
   outColor.b = fragTexCoord.y;
   //outColor.b = 0.5f
   //outColor.a = 1.0f
}