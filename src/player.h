#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "game.h"
#include "gameObject.h"
#include "shader.h"
#include "resource_manager.h"
#include "VoxelLoader.h"
#include "VoxelModel.h"
#include "level.h"

struct bullet: public GameObject {
	glm::vec3 direction;

	bullet(glm::vec3 pos, glm::vec3 direction, float rotation) : direction(direction){
		this->pos = pos;
		this->rotate = glm::vec3(0, rotation, 0);
	}
};

class Player {
public:
	Game& game;
	VoxelModel& model;
	std::vector<bullet> bullets;

	//consts
	const float speed = 10.0f;
	const float bulletSpeed = 18.0f;
	const float bulletScale = 0.5f;

	//constructors
	Player(VoxelModel& model, Game& game, VoxelRenderer* renderer);

	//rendering
	void draw();
	void drawBullets();

	//bullets
	void fire();
	void moveBullets(float dt);
	float lastFireTime = 0.0f;
	float fireCooldown = 0.5f;
	
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