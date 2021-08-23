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
	void takeDamage();
	void powerUp();
	float getLastDashTime() { return lastDashTime; }
	
private:
	void fire();

	//movement
	void movePlayer(float dt);
	void moveVertical(float dt);
	void rotatePlayer(float dt);
	bool grounded = false;
	float verticalVelocity   = -0.1f;
	const float dashCooldown = 5.0f;
	float lastDashTime       = 0.0f;
	float dashVelocity       = 0.1f;
	glm::vec3 dashDirection  = glm::vec3(0.0f);

	//powerup
	bool poweredUp = false;
	float powerUpDuration = 10.0f;
	float lastPowerUpTime = 0.0f;

	//audio
	static AudioPlayer shootAudio;
	static AudioPlayer dashAudio;
	static AudioPlayer movementAudio;
	static AudioPlayer damageAudio;
};