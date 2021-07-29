#version 330 core

#if __VERSION__ < 130
#define TEXTURE2D texture2D
#else
#define TEXTURE2D texture
#endif

in vec2 texCoord;
out vec4 FragColor;

uniform sampler2D voxTexture;
uniform vec3 voxColor;

void main()
{
    FragColor = vec4(voxColor, 1.0) * TEXTURE2D(voxTexture, texCoord);
}