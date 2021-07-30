#version 330 core

#if __VERSION__ < 130
#define TEXTURE2D texture2D
#else
#define TEXTURE2D texture
#endif

in vec2 texCoord;
in vec3 Normal;
in vec3 fragPos;
out vec4 FragColor;

uniform sampler2D voxTexture;
uniform vec3 voxColor;
uniform vec3 lightPos;
uniform vec3 lightColor;

void main()
{
    //ambient lighting calculations
    float ambientStrength = 0.5;
    vec3 ambient = ambientStrength * lightColor;

    //diffuse lighting calculations
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - fragPos);  
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 result = (ambient + diffuse) * voxColor;
    FragColor = vec4(result, 1.0) * TEXTURE2D(voxTexture, texCoord);
}