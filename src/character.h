#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

#include "gameObject.h"

//FORWARD DECLARATIONS
class Game;
class VoxelModel;
class VoxelRenderer;

struct bullet {
	float x, y, z;   //position
	float R, G, B;   //color
	float rotation;  //rotation around y-axis
	glm::vec3 direction;  //direction of movement

	bullet(glm::vec3 pos, glm::vec3 direction, glm::vec3 color, float rotation) : direction(direction) {
		x = pos.x;
		y = pos.y;
		z = pos.z;
		R = color.x;
		G = color.y;
		B = color.z;
		this->rotation = rotation;
	}
};

class Character: public GameObject{
public:
	Game& game;
	VoxelModel& model;
	VoxelRenderer& renderer;
	std::vector<bullet> bullets;

	virtual void updateState() = 0;

	//consts
	const float speed = 10.0f;
	const float bulletSpeed = 18.0f;
	const float bulletScale = 0.5f;

	//constructors
	Character(VoxelModel& model, Game& game, VoxelRenderer& renderer) : GameObject(), model(model), game(game), renderer(renderer){}

	//rendering
	void draw();
	void drawBullets();

	//bullets
	virtual void fire() = 0;
	void moveBullets(float dt);
	float lastFireTime = 0.0f;
	float fireCooldown = 0.5f;
};