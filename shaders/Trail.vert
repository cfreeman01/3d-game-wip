#version 330 core

//regular vertex attributes
layout (location = 0) in vec3 posVec;
layout (location = 1) in vec3 normalVec;

//instanced attributes
layout (location = 2) in vec3 worldPos;
layout (location = 3) in vec3 inColor;
layout (location = 4) in float life;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

out float alpha;
out vec3 voxColor;

void main()
{
    mat4 translation = mat4(
      vec4(1.0, 0.0, 0.0, 0.0),
      vec4(0.0, 1.0, 0.0, 0.0),
      vec4(0.0, 0.0, 1.0, 0.0),
      vec4(worldPos, 1.0)
      );

    //calculate outputs
    gl_Position = projection * view * translation * model * vec4(posVec, 1.0);  
    alpha = life;
    voxColor = inColor;
}