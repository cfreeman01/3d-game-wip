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
#include "levelRenderer.h"

using namespace std;

audioPlayer Game::gameAudio;
VoxelModel first_island;

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
    first_island = VoxelLoader::loadModel("models/first_island.vox", "first_island");
    first_island.renderer = new VoxelRenderer(ResourceManager::GetShader("VoxelShader"), *this);
    first_island.scale = 1.0f;
    //load level
    lRenderer = new levelRenderer(ResourceManager::GetShader("VoxelShader"), *this, *first_island.renderer);
    //create camera
    mainCamera = new Camera(glm::vec3(-38.3f, 21.8f, -21.7f), glm::vec3(0.0f, 1.0f, 0.0f), 29.0f, -28.6f);

    first_island.pos = glm::vec3(-(float)first_island.sizeX / 2, -(float)first_island.sizeY / 2, -(float)first_island.sizeZ / 2);
}

void Game::Update(float dt)
{
    elapsedTime += dt;
}

void Game::ProcessInput(float dt)
{
    if (Keys[GLFW_KEY_1] && elapsedTime - lastCameraModeSwitch >= 0.5f) {
        mainCamera->freeMode = !mainCamera->freeMode;
        lastCameraModeSwitch = elapsedTime;
    }

    if (mainCamera->freeMode) {
        //process keyboard input
        if (Keys[GLFW_KEY_W])
            mainCamera->ProcessKeyboard(FORWARD, dt);
        if (Keys[GLFW_KEY_A])
            mainCamera->ProcessKeyboard(LEFT, dt);
        if (Keys[GLFW_KEY_S])
            mainCamera->ProcessKeyboard(BACKWARD, dt);
        if (Keys[GLFW_KEY_D])
            mainCamera->ProcessKeyboard(RIGHT, dt);
        //process mouse input
        mainCamera->ProcessMouseMovement(mouseX, mouseY);
    }
    else {
        //possibly rotate camera
        if (Keys[GLFW_KEY_Q])
            mainCamera->rotate(dt, false);
        if (Keys[GLFW_KEY_E] )
            mainCamera->rotate(dt, true);
        mainCamera->ProcessMouseScroll(mouseWheelOffset);
        mouseWheelOffset = 0.0f;
    }
}

void Game::Render(float dt)
{
    lRenderer->drawLightSource();
    first_island.draw();
}