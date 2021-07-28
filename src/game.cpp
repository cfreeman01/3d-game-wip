#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/geometric.hpp>
#include <iostream>
#include <string>
#include "game.h"
#include "shader.h"
#include "resource_manager.h"
using namespace std;

audioPlayer Game::gameAudio;

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

}

void Game::Update(float dt)
{

}

void Game::ProcessInput(float dt)
{

}

void Game::Render(float dt)
{

}