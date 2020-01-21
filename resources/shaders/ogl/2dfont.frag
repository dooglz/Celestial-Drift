#version 410 core

uniform sampler2D font_texture;

in vec4 fragCol;
in vec2 fragTexCoord;
out vec4 outColor;

void main()
{
   float a = texture(font_texture, fragTexCoord).r;
   float c =  max(0.0,min(1.0,a));
   outColor= vec4(fragCol.rgb*c,c);
 //  outColor= vec4(1.0,1.0,0.0,1.);
}
