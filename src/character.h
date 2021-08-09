#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <list>

#include "gameObject.h"
#include "trailGenerator.h"

//FORWARD DECLARATIONS
class Game;
class VoxelModel;
class VoxelRenderer;

struct bullet: public GameObject {
	glm::vec3 color;
	glm::vec3 direction;  //direction of movement
	TrailGenerator trail;

	bullet(glm::vec3 pos, glm::vec3 direction, glm::vec3 color, float rotation, float scale): color(color), direction(direction) {
		this->pos = pos;
		this->rotate.y = rotation;
		this->scale = scale;
		trail = TrailGenerator(this, color);
	}
};

/*base class for the player and all enemies*/
class Character: public GameObject{
public:
	Game& game;
	virtual void updateState(float dt) = 0;

	//models and animation
	std::vector<VoxelModel*> charModels;  //Voxel models that the character cycles through
	int modelIndex = 0;                   //index into charModels
	float lastModelUpdate = 0.0f;
	float modelUpdateDelay; 

	//movement speed
	float speed;

	//constructors
	Character(Game& game, VoxelRenderer& renderer) : GameObject(), game(game), renderer(renderer){}

	//rendering
	VoxelRenderer& renderer;
	void draw();
	void drawBullets();

	//bullets
	std::list<bullet> bullets;
	virtual void fire() = 0;
	void moveBullets(float dt);
	float lastFireTime = 0.0f;
	float fireCooldown = 0.5f;
	float bulletSpeed;
	float bulletScale;
};