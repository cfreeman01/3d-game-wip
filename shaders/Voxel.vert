#version 330 core

//regular vertex attributes
layout (location = 0) in vec3 posVec;
layout (location = 1) in vec3 normalVec;

//instanced attributes
layout (location = 2) in vec3 offset;
layout (location = 3) in vec3 inColor;

uniform mat4 model1;
uniform mat4 model2;
uniform mat4 projection;
uniform mat4 view;
uniform float modelScale;

out vec3 Normal;
out vec3 fragPos;
out vec3 voxColor;

void main()
{
    /*for voxel models, this matrix is the offset from the model position
    in other cases, it is just the position of the voxel and modelScale = 1.0*/
    mat4 translation = mat4(
      vec4(1.0, 0.0, 0.0, 0.0),
      vec4(0.0, 1.0, 0.0, 0.0),
      vec4(0.0, 0.0, 1.0, 0.0),
      vec4(modelScale * offset, 1.0)
      );

    //calculate outputs
    gl_Position = projection * view * model2 * translation * model1 * vec4(posVec, 1.0);  
    Normal = mat3(transpose(inverse(model2 * translation * model1))) * normalVec;
    fragPos = vec3(model2 * translation * model1 * vec4(posVec, 1.0));
    voxColor = inColor;
}