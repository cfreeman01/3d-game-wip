#version 330 core

in float alpha;
in vec3 voxColor;
out vec4 FragColor;

void main()
{
    FragColor = vec4(voxColor, alpha);
}