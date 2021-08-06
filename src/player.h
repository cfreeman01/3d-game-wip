#pragma once
#include "character.h"

//FORWARD DECLARATIONS 
class Game;
class VoxelModel;
class AudioPlayer;

class Player: public Character {
public:
	//constructors
	Player(VoxelModel& model, Game& game);

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