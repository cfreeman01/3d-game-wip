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
	Player(Game& game, VoxelRenderer& renderer);

	static void loadModels();
	void updateState(float dt);
	void processInput(float dt);
	void fire();
	
	//movement
	void takeDamage();
	void movePlayer(float dt);
	void moveVertical(float dt);
	void rotatePlayer(float dt);
	bool grounded = false;
	const float dashCooldown = 5.0f;
	float lastDashTime       = 0.0f;
	float dashVelocity       = 0.1f;
	glm::vec3 dashDirection  = glm::vec3(0.0f);
	float verticalVelocity   = -0.1f;

	//audio
	static AudioPlayer shootAudio;
	static AudioPlayer movementAudio;
	static AudioPlayer damageAudio;
};