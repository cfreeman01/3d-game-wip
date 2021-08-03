#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "game.h"
#include "shader.h"
#include "resource_manager.h"
#include "VoxelLoader.h"
#include "VoxelModel.h"

struct bullet {
	glm::vec3 pos;
	glm::vec3 direction;
	float rotation;

	bullet(glm::vec3 pos, glm::vec3 direction, float rotation) : pos(pos), direction(direction), rotation(rotation){}
};

class Player {
public:
	Game& game;
	VoxelModel& model;
	std::vector<bullet> bullets;

	//consts
	const float speed = 10.0f;
	const float bulletSpeed = 10.0f;
	const float bulletScale = 0.5f;

	//constructors
	Player(VoxelModel& model, Game& game, VoxelRenderer* renderer);

	//rendering
	void draw();
	void drawBullets();

	//input
	void fire();
	void moveBullets(float dt);
	void processInput(float dt);
};