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
#include "camera.h"
#include "enemy.h"
#include "enemy1.h"
#include "enemy2.h"
#include "enemy3.h"
#include "boss.h"
#include "hud.h"

using namespace std;

bool displayFrames = true;
float lastDisplayTime = 0.0f;

Game::Game(unsigned int width, unsigned int height)
    : State(GAME_ACTIVE), Keys(), Width(width), Height(height)
{
    mouseX = (float)this->Width / 2;
    mouseY = (float)this->Height / 2;
    mouse1 = mouse2 = false;
}

Game::~Game()
{
}

void Game::Init()
{
    //load models
    Player::loadModels();
    Enemy1::loadModels();
    Enemy2::loadModels();
    Enemy3::loadModels();
    Boss::loadModels();
    Level::loadModels();

    //load textures
    ResourceManager::LoadTexture("textures/cursor.png", true, "cursor");
    ResourceManager::LoadTexture("textures/gameOver.png", true, "GameOver");
    ResourceManager::LoadTexture("textures/controls.png", true, "controls");
    HUD::loadTextures();

    //load audio
    audioEngine.init();
    restartAudio.load("audio/restart.wav");
    gameOverAudio.load("audio/game_over.wav");
    swoosh1.load("audio/swoosh_1.wav");
    swoosh2.load("audio/swoosh_2.wav");
    Enemy::loadAudio();
    Enemy1::loadAudio();
    Enemy2::loadAudio();
    Enemy3::loadAudio();

    //load shaders
    Shader &voxShader = ResourceManager::LoadShader("shaders/Voxel.vert", "shaders/Voxel.frag", nullptr, "VoxelShader");
    Shader &spriteShader = ResourceManager::LoadShader("shaders/Sprite.vert", "shaders/Sprite.frag", nullptr, "SpriteShader");
    ResourceManager::LoadShader("shaders/Trail.vert", "shaders/Trail.frag", nullptr, "TrailShader");

    //create voxel renderer
    vRenderer = new VoxelRenderer(voxShader, *this);

    //create sprite renderer
    sRenderer = new SpriteRenderer(spriteShader, *this);

    //create camera
    mainCamera = new Camera(1.5f * glm::vec3(-38.3f, 21.8f, -21.7f), glm::vec3(0.0f, 1.0f, 0.0f), 29.0f, -28.6f);
    voxShader.SetMatrix4("projection", mainCamera->GetProjectionMatrix());

    //load level
    currentLevel = new Level(*vRenderer, *this);

    //load player object
    player = new Player(*this, *vRenderer);
    player->pos = glm::vec3(-8.0f, -1.0f, -1.5f);
    player->scale = 0.1;

    //initialize HUD
    hud = new HUD(*this, *sRenderer);
}

void Game::Update(float dt)
{
    if (State == GAME_OVER)
        return;

    elapsedTime += dt;

    mainCamera->rotate(dt);

    player->updateState(dt);

    currentLevel->updateState(dt);

    if (player->getState() == Character::DEAD)
    {
        audioEngine.play(gameOverAudio);
        State = GAME_OVER;
        return;
    }

    if (displayFrames && elapsedTime - lastDisplayTime > 1.0f)
    {
        lastDisplayTime = elapsedTime;
        std::cout << 1 / dt << "FPS\n";
    }
}

void Game::ProcessInput(float dt)
{
    if (State == GAME_OVER)
    {
        if (mouse1)
        { //on game over, player presses mouse1 to restart game
            restart();
            State = GAME_ACTIVE;
        }
        return;
    }

    //Process camera input
    if (mainCamera->freeMode)
    {
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

    else
    {
        //rotate camera
        if (Keys[GLFW_KEY_Q] && mainCamera->rotating == 0)
        {
            mainCamera->rotating = -1;
            audioEngine.play(swoosh1);
        }
        if (Keys[GLFW_KEY_E] && mainCamera->rotating == 0)
        {
            mainCamera->rotating = 1;
            audioEngine.play(swoosh2);
        }
        //zoom camera
        mainCamera->ProcessMouseScroll(mouseWheelOffset);
        mouseWheelOffset = 0.0f;

        //Process player input
        if (player->getState() == Character::ALIVE)
            player->processInput(dt);
    }

    if (Keys[GLFW_KEY_ESCAPE])
    {
        exit(0);
    }
}

void Game::Render(float dt)
{
    currentLevel->draw();

    player->draw();

    //draw cursor
    sRenderer->DrawSprite(ResourceManager::GetTexture("cursor"), glm::vec2(mouseX, mouseY));

    hud->draw();

    //display game controls for first ten seconds
    if (elapsedTime <= 10.0f)
    {
        float controlsSize = Width/4.0f;
        sRenderer->DrawSprite(ResourceManager::GetTexture("controls"), glm::vec2(Width - controlsSize, 0),
                              glm::vec2(controlsSize, controlsSize));
    }

    //draw a game over message
    if (State == GAME_OVER)
    {
        float gameOverSize = Width/2.25f;
        sRenderer->DrawSprite(ResourceManager::GetTexture("GameOver"), glm::vec2(Width / 2 - gameOverSize / 2, Height / 2 - gameOverSize / 2),
                              glm::vec2(gameOverSize, gameOverSize));
    }
}

void Game::restart()
{
    audioEngine.stopAll();
    audioEngine.play(restartAudio);
    //reset level
    delete currentLevel;
    currentLevel = new Level(*vRenderer, *this);

    //reset timing
    elapsedTime = 0.0f;
    lastDisplayTime = 0.0f;

    //reset player
    delete player;
    player = new Player(*this, *vRenderer);
    player->pos = glm::vec3(-8.0f, -2.0f, -1.5f);
    player->scale = 0.1;
}