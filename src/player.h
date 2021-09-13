#pragma once
#include "character.h"
#include "soloud.h"
#include "soloud_wav.h"

//FORWARD DECLARATIONS
class Game;
class VoxelModel;
class VoxelRenderer;

/*object representing the player character*/
class Player : public Character
{
public:
	Player(Game &game, VoxelRenderer &renderer);

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
	float verticalVelocity = -0.1f;
	const float dashCooldown = 5.0f;
	float lastDashTime = 0.0f;
	float dashVelocity = 75.0f;
	glm::vec3 dashDirection = glm::vec3(0.0f);

	//audio
	SoLoud::Wav shootAudio;
	SoLoud::Wav damageAudio;
	SoLoud::Wav dashAudio;
	SoLoud::Wav deathAudio;
	SoLoud::Wav jumpAudio;
	SoLoud::Wav landAudio;

	//powerup
	bool poweredUp = false;
	float powerUpDuration = 10.0f;
	float lastPowerUpTime = 0.0f;
};