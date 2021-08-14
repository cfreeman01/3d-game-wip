#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

//FORWARD DECLARATIONS
class Level;
class VoxelRenderer;
class Character;
class Player;
class SpriteRenderer;
class Camera;
class AudioPlayer;

// Represents the current state of the game
enum GameState {
    GAME_ACTIVE,
    GAME_OVER,
    GAME_WIN
};

// Game holds all game-related state and functionality.
// Combines all game-related data into a single class for
// easy access to each of the components and manageability.
class Game
{
public:
    GameState               State;
    Level*                  currentLevel;
    VoxelRenderer*          vRenderer;
    SpriteRenderer*         sRenderer;
    Camera*                 mainCamera;
    Player*                 player;
    //input
    bool                    Keys[1024];
    bool                    mouse1 = false, mouse2 = false;
    float                   mouseX, mouseY;
    float                   mouseWheelOffset = 0.0f;
    float                   lastCameraModeSwitch = 0.0f;
    //dimensions
    int                     Width, Height;
    int                     playAreaHeight;
    //timing
    float                   elapsedTime = 0.0f;
    // constructor/destructor
    Game(unsigned int width, unsigned int height);
    ~Game();
    // initialize game state (load all shaders/textures/levels)
    void Init();
    // game loop
    void ProcessInput(float dt);
    void Update(float dt);
    void Render(float dt);
    void restart();
    //audio
    static AudioPlayer gameAudio;
};