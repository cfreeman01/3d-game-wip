#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include "VoxelRenderer.h"
#include "VoxelModel.h"
#include "resource_manager.h"
#include "character.h"
#include "shader.h"
#include "texture.h"
#include "game.h"
#include "camera.h"

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

    unsigned int normalsVBO;
    glGenBuffers(1, &normalsVBO);
    glBindBuffer(GL_ARRAY_BUFFER, normalsVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(voxelVertices), voxelVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    shader.Use();
    glBindVertexArray(0);
}

//draw a voxel that is part of a model
void VoxelRenderer::drawVoxelModel(VoxelModel& voxelModel) {
    shader.Use();
    glBindVertexArray(VAO);
	glm::mat4 model1 = glm::mat4(1.0f);
    glm::mat4 model2 = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
    
    float modelScale = voxelModel.scale;
    glm::vec3 size = glm::vec3(voxelModel.size.x, voxelModel.size.y, voxelModel.size.z);

    //calculate transformation matrices
    model1 = glm::scale(model1, glm::vec3(modelScale));
    model1 = glm::translate(model1, glm::vec3(0.5, 0.5, 0.5));

    model2 = glm::translate(model2, voxelModel.pos);
    model2 = glm::translate(model2, glm::vec3(size.x * 0.5 * modelScale, size.y * 0.5 * modelScale, size.z * 0.5 * modelScale));
    model2 = glm::rotate(model2, glm::radians(voxelModel.rotate.x), glm::vec3(1.0f, 0.0f, 0.0f));
    model2 = glm::rotate(model2, glm::radians(voxelModel.rotate.y), glm::vec3(0.0f, 1.0f, 0.0f));     
    model2 = glm::rotate(model2, glm::radians(voxelModel.rotate.z), glm::vec3(0.0f, 0.0f, 1.0f));
    model2 = glm::translate(model2, glm::vec3(-size.x * 0.5 * modelScale, -size.y * 0.5 * modelScale, -size.z * 0.5 * modelScale));

    view = game.mainCamera->GetViewMatrix();
    projection = game.mainCamera->GetProjectionMatrix();

    shader.SetMatrix4("model1", model1);
    shader.SetMatrix4("model2", model2);
    shader.SetMatrix4("view", view);
    shader.SetMatrix4("projection", projection);
    shader.SetFloat("modelScale", modelScale);

    //pass in instanced data
    unsigned int instanceVBO;
    glGenBuffers(1, &instanceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VoxelRenderData) * voxelModel.Voxels.size(), voxelModel.vRenderData, GL_STATIC_DRAW);

    //offset
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VoxelRenderData), (void*)0);
    glVertexAttribDivisor(2, 1);

    //color
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VoxelRenderData), (void*)(3 * sizeof(float)));
    glVertexAttribDivisor(3, 1);

    glDrawArraysInstanced(GL_TRIANGLES, 0, 36, voxelModel.Voxels.size());
    glBindVertexArray(0);
}

//draw a set of bullets for a character
void VoxelRenderer::drawBullets(Character& character) {
    shader.Use();
    glBindVertexArray(VAO);
    glm::mat4 model1 = glm::mat4(1.0f);
    glm::mat4 model2 = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);

    model1 = glm::scale(model1, glm::vec3(character.bulletScale));
    model1 = glm::translate(model1, glm::vec3(0.5, 0.5, 0.5));

    view = game.mainCamera->GetViewMatrix();
    projection = game.mainCamera->GetProjectionMatrix();

    shader.SetMatrix4("model1", model1);
    shader.SetMatrix4("model2", model2);
    shader.SetMatrix4("view", view);
    shader.SetMatrix4("projection", projection);
    shader.SetFloat("modelScale", 1.0f);

    //pass in instanced data
    unsigned int instanceVBO;
    glGenBuffers(1, &instanceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(bullet) * character.bullets.size(), &character.bullets[0], GL_STATIC_DRAW);

    //bullet position
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(bullet), (void*)0);
    glVertexAttribDivisor(2, 1);

    //color
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(bullet), (void*)(3 * sizeof(float)));
    glVertexAttribDivisor(3, 1);

    glDrawArraysInstanced(GL_TRIANGLES, 0, 36, character.bullets.size());
    glBindVertexArray(0);
}
