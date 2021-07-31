#include "VoxelRenderer.h"
#include "VoxelModel.h"
#include "resource_manager.h"
#include <glm/glm.hpp>
#include <glad/glad.h>

float voxelVertices[] = {
    //vertices              //normals
     -0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,

    -0.5f, -0.5f,  0.5f,    0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,    0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,    0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,    0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,    0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,    0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f,   -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,   -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,   -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,   -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,   -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,   -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,    1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,    1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,    1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,    1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,    1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,    1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f
};
float voxelTexCoords[] = {  //texture coordinates
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

VoxelRenderer::VoxelRenderer(Shader& shader, Game& game): shader(shader), game(game) {
    initRenderData();
}

void VoxelRenderer::initRenderData() {
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int pointsVBO;
    glGenBuffers(1, &pointsVBO);
    glBindBuffer(GL_ARRAY_BUFFER, pointsVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(voxelVertices), voxelVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    unsigned int textureVBO;
    glGenBuffers(1, &textureVBO);
    glBindBuffer(GL_ARRAY_BUFFER, textureVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(voxelTexCoords), voxelTexCoords, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);

    unsigned int normalsVBO;
    glGenBuffers(1, &normalsVBO);
    glBindBuffer(GL_ARRAY_BUFFER, normalsVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(voxelVertices), voxelVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);

    shader.Use();
    shader.SetInteger("voxTexture", 0);
    glBindVertexArray(0);
}

//draw a voxel that is part of a model
void VoxelRenderer::drawVoxel(Voxel& voxel, VoxelModel& voxelModel) {
    shader.Use();
    ResourceManager::GetTexture("empty").Bind();
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);

    float modelScale = voxelModel.scale;
    glm::vec3 size = glm::vec3(voxelModel.sizeX, voxelModel.sizeY, voxelModel.sizeZ);

    //TRANSFORMATIONS----------------
    model = glm::translate(model, voxelModel.pos);

    model = glm::translate(model, glm::vec3(size.x * 0.5 * modelScale, size.y * 0.5 * modelScale, size.z * 0.5 * modelScale));
    model = glm::rotate(model, glm::radians(voxelModel.rotateX), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(voxelModel.rotateY), glm::vec3(0.0f, 1.0f, 0.0f));     
    model = glm::rotate(model, glm::radians(voxelModel.rotateZ), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(-size.x * 0.5 * modelScale, -size.y * 0.5 * modelScale, -size.z * 0.5 * modelScale));

    model = glm::translate(model, glm::vec3(voxel.x * modelScale, voxel.y * modelScale, voxel.z * modelScale));

    model = glm::scale(model, glm::vec3(modelScale));
    //--------------------------------

    projection = game.mainCamera->GetProjectionMatrix();
    view = game.mainCamera->GetViewMatrix();

    shader.SetMatrix4("model", model);
    shader.SetMatrix4("view", view);
    shader.SetMatrix4("projection", projection);

    //get color
    unsigned int colorInt = voxelModel.palette[voxel.colorIndex];
    unsigned int R = (colorInt & 0x000000ff);
    unsigned int G = (colorInt & 0x0000ff00) >> 8;
    unsigned int B = (colorInt & 0x00ff0000) >> 16;
    shader.SetVector3f("voxColor", glm::vec3((float)R/255, (float)G/255, (float)B/255));

    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

//draw a standalone voxel
void VoxelRenderer::drawVoxel(glm::vec3 pos, glm::vec3 color, float scale) {
    shader.Use();
    ResourceManager::GetTexture("empty").Bind();
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);

    model = glm::translate(model, pos);
    model = glm::scale(model, glm::vec3(scale));
    projection = game.mainCamera->GetProjectionMatrix();
    view = game.mainCamera->GetViewMatrix();

    shader.SetMatrix4("model", model);
    shader.SetMatrix4("view", view);
    shader.SetMatrix4("projection", projection);
    shader.SetVector3f("voxColor", color);

    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}