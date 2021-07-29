#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/geometric.hpp>
#include <iostream>
#include <string>
#include "game.h"
#include "shader.h"
#include "resource_manager.h"
#include "VoxelLoader.h"
using namespace std;

audioPlayer Game::gameAudio;
VoxelModel test;

Game::Game(unsigned int width, unsigned int height)
    : State(GAME_ACTIVE), Keys(), Width(width), Height(height)
{
    mouseX = (float)this->Width  / 2;
    mouseY = (float)this->Height / 2;
    mouse1 = mouse2 = false;

    playAreaHeight = Height - (Height / 5);
}

Game::~Game()
{

}

void Game::Init()
{
    //load textures
    ResourceManager::LoadTexture("textures/empty.png", true, "empty");
    //load shaders
    ResourceManager::LoadShader("shaders/Voxel.vert", "shaders/Voxel.frag", nullptr, "VoxelShader");
    //load models
    test = VoxelLoader::loadModel("models/test.vox", "test");
    test.renderer = new VoxelRenderer(ResourceManager::GetShader("VoxelShader"));
    test.scale = 1.0f;
    test.pos = glm::vec3(0.0f, 0.0f, 0.0f);
    //load level
    currentLevel = new level(ResourceManager::GetShader("VoxelShader"));
}

void Game::Update(float dt)
{

}

void Game::ProcessInput(float dt)
{

}

void Game::Render(float dt)
{
    currentLevel->drawGround();
    test.rotateY += dt*100;
    test.draw();
}