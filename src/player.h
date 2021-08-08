#pragma once
#include "character.h"

//FORWARD DECLARATIONS 
class Game;
class VoxelModel;
class VoxelRenderer;
class AudioPlayer;

/*object representing the player character*/
class Player: public Character {
public:
	void updateState(float dt);
	static void loadModels();

	//constructors
	Player(Game& game, VoxelRenderer& renderer);

	//input
	void fire();
	
	//movement/collisions
	void processInput(float dt);
	void movePlayer(float dt);
	void moveVertical(float dt);
	void rotatePlayer(float dt);
	bool grounded = false;
	float verticalVelocity = -0.1f;

	//audio
	static AudioPlayer shootAudio;
	static AudioPlayer movementAudio;
};