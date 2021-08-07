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
#include "VoxelRenderer.h"
#include "VoxelModel.h"
#include "spriteRenderer.h"
#include "level.h"
#include "character.h"
#include "player.h"
#include "audioPlayer.h"
#include "camera.h"

using namespace std;

AudioPlayer Game::gameAudio;

bool displayFrames = true;
float lastDisplayTime = 0.0f;

Game::Game(unsigned int width, unsigned int height)
    : State(GAME_ACTIVE), Keys(), Width(width), Height(height)
{
    mouseX = (float)this->Width  / 2;
    mouseY = (float)this->Height / 2;
    mouse1 = mouse2 = false;

    playAreaHeight = Height;
}

Game::~Game()
{

}

void Game::Init()
{
    //load textures
    ResourceManager::LoadTexture("textures/cursor.png", true, "cursor");
    //load shaders
    Shader& voxShader = ResourceManager::LoadShader("shaders/Voxel.vert", "shaders/Voxel.frag", nullptr, "VoxelShader");
    Shader& spriteShader = ResourceManager::LoadShader("shaders/Sprite.vert", "shaders/Sprite.frag", nullptr, "SpriteShader");
    //create voxel renderer
    vRenderer = new VoxelRenderer(voxShader, *this);
    //create sprite renderer
    sRenderer = new SpriteRenderer(spriteShader, *this);
    //create camera
    mainCamera = new Camera(glm::vec3(-38.3f, 21.8f, -21.7f), glm::vec3(0.0f, 1.0f, 0.0f), 29.0f, -28.6f);
    voxShader.SetMatrix4("projection", mainCamera->GetProjectionMatrix());
    //load level
    currentLevel = new Level(*vRenderer, *this);
    //load models
    Player::loadModels(vRenderer);
    VoxelLoader::loadModel("models/enemy1.vox", "enemy1");
    //load player object
    player = new Player(VoxelLoader::getModel("player_0"), *this, *vRenderer);
    player->pos = glm::vec3(-8.0f, -2.0f, -1.5f);
    player->scale = 0.1;
}

void Game::Update(float dt)
{
    elapsedTime += dt;

    mainCamera->rotate(dt);

    player->moveBullets(dt);

    if (displayFrames && elapsedTime - lastDisplayTime > 1.0f) {
        lastDisplayTime = elapsedTime;
        std::cout << 1 / dt << "FPS\n";
    }
}

void Game::ProcessInput(float dt)
{
    if (Keys[GLFW_KEY_1] && elapsedTime - lastCameraModeSwitch >= 0.5f) {
        mainCamera->freeMode = !mainCamera->freeMode;
        lastCameraModeSwitch = elapsedTime;
    }

    //Process camera input
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
        if (Keys[GLFW_KEY_Q] && mainCamera->rotating == 0) {
            mainCamera->rotating = -1;
            gameAudio.play("audio/swoosh_1.mp3");
        }
        if (Keys[GLFW_KEY_E] && mainCamera->rotating == 0) {
            mainCamera->rotating = 1;
            gameAudio.play("audio/swoosh_2.mp3");
        }
        mainCamera->ProcessMouseScroll(mouseWheelOffset);
        mouseWheelOffset = 0.0f;

        //Process player input
        player->processInput(dt);
    }
}

void Game::Render(float dt)
{
    player->draw();
    currentLevel->drawIslands();
    //draw cursor
    sRenderer->DrawSprite(ResourceManager::GetTexture("cursor"), glm::vec2(mouseX, mouseY));
}