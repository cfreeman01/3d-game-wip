#include "VoxelRenderer.h"
#include <glm/glm.hpp>
#include <glad/glad.h>

float vertices[] = { //cube vertices
	-0.1f, -0.1f, -0.1f,
 0.1f, -0.1f, -0.1f,
 0.1f,  0.1f, -0.1f,
 0.1f,  0.1f, -0.1f,
-0.1f,  0.1f, -0.1f,
-0.1f, -0.1f, -0.1f,

-0.1f, -0.1f,  0.1f,
 0.1f, -0.1f,  0.1f,
 0.1f,  0.1f,  0.1f,
 0.1f,  0.1f,  0.1f,
-0.1f,  0.1f,  0.1f,
-0.1f, -0.1f,  0.1f,

-0.1f,  0.1f,  0.1f,
-0.1f,  0.1f, -0.1f,
-0.1f, -0.1f, -0.1f,
-0.1f, -0.1f, -0.1f,
-0.1f, -0.1f,  0.1f,
-0.1f,  0.1f,  0.1f,

 0.1f,  0.1f,  0.1f,
 0.1f,  0.1f, -0.1f,
 0.1f, -0.1f, -0.1f,
 0.1f, -0.1f, -0.1f,
 0.1f, -0.1f,  0.1f,
 0.1f,  0.1f,  0.1f,

-0.1f, -0.1f, -0.1f,
 0.1f, -0.1f, -0.1f,
 0.1f, -0.1f,  0.1f,
 0.1f, -0.1f,  0.1f,
-0.1f, -0.1f,  0.1f,
-0.1f, -0.1f, -0.1f,

-0.1f,  0.1f, -0.1f,
 0.1f,  0.1f, -0.1f,
 0.1f,  0.1f,  0.1f,
 0.1f,  0.1f,  0.1f,
-0.1f,  0.1f,  0.1f,
-0.1f,  0.1f, -0.1f,
};
float textureCoords[] = {  //texture coordinates
 0.0f, 0.0f,
 1.0f, 0.0f,
 1.0f, 1.0f,
 1.0f, 1.0f,
 0.0f, 1.0f,
 0.0f, 0.0f,

 0.0f, 0.0f,
 1.0f, 0.0f,
 1.0f, 1.0f,
 1.0f, 1.0f,
 0.0f, 1.0f,
 0.0f, 0.0f,

 1.0f, 0.0f,
 1.0f, 1.0f,
 0.0f, 1.0f,
 0.0f, 1.0f,
 0.0f, 0.0f,
 1.0f, 0.0f,

 1.0f, 0.0f,
 1.0f, 1.0f,
 0.0f, 1.0f,
 0.0f, 1.0f,
 0.0f, 0.0f,
 1.0f, 0.0f,

 0.0f, 1.0f,
 1.0f, 1.0f,
 1.0f, 0.0f,
 1.0f, 0.0f,
 0.0f, 0.0f,
 0.0f, 1.0f,

 0.0f, 1.0f,
 1.0f, 1.0f,
 1.0f, 0.0f,
 1.0f, 0.0f,
 0.0f, 0.0f,
 0.0f, 1.0f
};

VoxelRenderer::VoxelRenderer(Shader& shader): shader(shader) {
    initRenderData();
}

void VoxelRenderer::initRenderData() {
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int pointsVBO;
    glGenBuffers(1, &pointsVBO);
    glBindBuffer(GL_ARRAY_BUFFER, pointsVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    unsigned int textureVBO;
    glGenBuffers(1, &textureVBO);
    glBindBuffer(GL_ARRAY_BUFFER, textureVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(textureCoords), textureCoords, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
}

void VoxelRenderer::drawVoxel(Voxel& voxel) {
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);

    model = glm::translate(model, glm::vec3(voxel.x,voxel.y,voxel.z));
    
}