#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <iostream>
#include "VoxelRenderer.h"
#include "VoxelModel.h"
#include "resource_manager.h"
#include "character.h"
#include "shader.h"
#include "texture.h"
#include "game.h"
#include "camera.h"
#include "trailGenerator.h"

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

/*draw a VoxelModel that is tied to a specific GameObject.
the GameObject holds the dynamic game data (like world position) while
the VoxelModel object holds the constant data obtained from a .VOX file*/
void VoxelRenderer::drawVoxelModel(VoxelModel& voxelModel, GameObject& object) {
    shader.Use();
    glBindVertexArray(VAO);
    glm::mat4 model1 = glm::mat4(1.0f);
    glm::mat4 model2 = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);

    float modelScale = object.scale;

    //calculate transformation matrices
    model1 = glm::scale(model1, glm::vec3(modelScale));
    model1 = glm::translate(model1, glm::vec3(0.5, 0.5, 0.5));

    model2 = glm::translate(model2, object.pos + glm::vec3(0, object.distanceOffGround, 0));
    model2 = glm::translate(model2, glm::vec3(voxelModel.size.x * 0.5 * modelScale, voxelModel.size.y * 0.5 * modelScale, voxelModel.size.z * 0.5 * modelScale));
    model2 = glm::rotate(model2, glm::radians(object.rotate.x), glm::vec3(1.0f, 0.0f, 0.0f));
    model2 = glm::rotate(model2, glm::radians(object.rotate.y), glm::vec3(0.0f, 1.0f, 0.0f));
    model2 = glm::rotate(model2, glm::radians(object.rotate.z), glm::vec3(0.0f, 0.0f, 1.0f));
    model2 = glm::translate(model2, glm::vec3(-voxelModel.size.x * 0.5 * modelScale, -voxelModel.size.y * 0.5 * modelScale, -voxelModel.size.z * 0.5 * modelScale));

    view = game.mainCamera->GetViewMatrix();
    projection = game.mainCamera->GetProjectionMatrix();

    shader.SetMatrix4("model1", model1);
    shader.SetMatrix4("model2", model2);
    shader.SetMatrix4("view", view);
    shader.SetMatrix4("projection", projection);
    shader.SetFloat("modelScale", modelScale);
    shader.SetVector3f("tintColor", object.tintColor);

    //pass in instanced data
    unsigned int instanceVBO;
    glGenBuffers(1, &instanceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VoxelRenderData) * voxelModel.numVoxels, voxelModel.vRenderData, GL_STATIC_DRAW);

    //offset
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VoxelRenderData), (void*)0);
    glVertexAttribDivisor(2, 1);

    //color
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VoxelRenderData), (void*)(3 * sizeof(float)));
    glVertexAttribDivisor(3, 1);

    glDrawArraysInstanced(GL_TRIANGLES, 0, 36, voxelModel.numVoxels);

    glDeleteBuffers(1, &instanceVBO);
    glBindVertexArray(0);
}

/*draw a set of bullets for a character*/
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
    shader.SetVector3f("tintColor", glm::vec3(1.0f,1.0f,1.0f));

    //make a contiguous data structure for the bullets
    std::vector<Character::bullet> bulletsVec;
    for (auto itr = character.bullets.begin(); itr != character.bullets.end(); itr++) {
        bulletsVec.push_back(*itr);
    }

    //pass in instanced data
    unsigned int instanceVBO;
    glGenBuffers(1, &instanceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Character::bullet) * bulletsVec.size(), &bulletsVec[0], GL_STATIC_DRAW);

    //bullet position
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Character::bullet), (void*)0);
    glVertexAttribDivisor(2, 1);

    //color
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Character::bullet), (void*)(sizeof(GameObject)));
    glVertexAttribDivisor(3, 1);

    glDrawArraysInstanced(GL_TRIANGLES, 0, 36, character.bullets.size());

    glDeleteBuffers(1, &instanceVBO);
    glBindVertexArray(0);
}

/*draw the particle trails for a character's bullets*/
void VoxelRenderer::drawTrails(Character& character) {
    ResourceManager::GetShader("TrailShader").Use();
    glBindVertexArray(VAO);
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);

    model = glm::scale(model, glm::vec3(character.bulletScale));
    model = glm::translate(model, glm::vec3(0.5, 0.5, 0.5));

    view = game.mainCamera->GetViewMatrix();
    projection = game.mainCamera->GetProjectionMatrix();

    ResourceManager::GetShader("TrailShader").SetMatrix4("model", model);
    ResourceManager::GetShader("TrailShader").SetMatrix4("view", view);
    ResourceManager::GetShader("TrailShader").SetMatrix4("projection", projection);

    //combine all bullet trails in a single vector
    std::vector<Particle> trails;
    for (auto itr = character.bullets.begin(); itr != character.bullets.end(); itr++) {
        trails.insert(trails.end(), itr->trail.particles.begin(), itr->trail.particles.end());
    }

    //pass in instanced data
    unsigned int instanceVBO;
    glGenBuffers(1, &instanceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Particle) * trails.size(), &trails[0], GL_STATIC_DRAW);

    //particle position
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)0);
    glVertexAttribDivisor(2, 1);

    //color
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)(3 * sizeof(float)));
    glVertexAttribDivisor(3, 1);

    //alpha value (particle's "life")
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)(6 * sizeof(float)));
    glVertexAttribDivisor(4, 1);

    glDrawArraysInstanced(GL_TRIANGLES, 0, 36, trails.size());

    glDeleteBuffers(1, &instanceVBO);
    glBindVertexArray(0);
}
