#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <list>

#include "gameObject.h"
#include "trailGenerator.h"


enum CharState { //character state
	ALIVE,
	DYING,
	DEAD
};

//FORWARD DECLARATIONS
class Game;
class VoxelModel;
class VoxelRenderer;

struct bullet: public GameObject {
	glm::vec3 color;
	glm::vec3 direction;  //direction of movement
	TrailGenerator trail;

	bullet(glm::vec3 pos, glm::vec3 direction, glm::vec3 color, float rotation, float scale, float numParticles): color(color), direction(direction) {
		this->pos = pos;
		this->rotate.y = rotation;
		this->scale = scale;
		trail = TrailGenerator(this, color, numParticles);
	}
};

/*base class for player and enemies*/
class Character: public GameObject{
public:
	Game& game;
	virtual void updateState(float dt) = 0;

	//models and animation
	std::vector<VoxelModel*> charModels;  //models for regular animation
	std::vector<VoxelModel*> deathModels; //models for death animation
	int modelIndex = 0;                   //index into charModels
	float lastModelUpdate = 0.0f;
	float modelUpdateDelay; 
	void nextModel();

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

	//damage/hp
	CharState cState = ALIVE;
	int hp = 3;
	virtual void takeDamage() = 0;
	float lastDamaged = 0.0f;  //time at which character last took damage
	float tintDuration = 0.4f; //how long model should be red after taking damage
};