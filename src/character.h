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

/*base class for player and enemies*/
class Character: public GameObject
{
public:
	enum State  //character state
	{ 
		ALIVE,
		DYING,
		DEAD
	};

	/*represents a single bullet fired by the character*/
	struct bullet : public GameObject {
		glm::vec3 color;
		glm::vec3 direction;  //direction of movement
		TrailGenerator trail;

		bullet(glm::vec3 pos, glm::vec3 direction, glm::vec3 color, float rotation, float scale, float numParticles) : color(color), direction(direction) {
			this->pos = pos;
			this->rotate.y = rotation;
			this->scale = scale;
			trail = TrailGenerator(this, color, numParticles);
		}
	};

protected:
	Game& game;
	State state = ALIVE;
	friend class VoxelRenderer;

	//models and animation
	VoxelRenderer& renderer;
	std::vector<VoxelModel*> charModels;  //models for regular animation
	std::vector<VoxelModel*> deathModels; //models for death animation
	int modelIndex = 0;                   //index into charModels
	float lastModelUpdate = 0.0f;
	float modelUpdateDelay;
	void nextModel();
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
	int hp = 3;
	virtual void takeDamage() = 0;
	float lastDamaged = 0.0f;  //time at which character last took damage
	float tintDuration = 0.4f; //how long model should be red after taking damage

	//other properties
	float speed;   //movement speed

public:
	Character(Game& game, VoxelRenderer& renderer) : GameObject(), game(game), renderer(renderer){}

	//state
	State getState() { return state; }
	virtual void updateState(float dt) = 0;

	//rendering
	void draw();

	//hp
	int getHP() { return hp; }
	void incHP() { if (hp < 3) hp++; }
	void decHP() { if (hp > 0) hp--; }

	//bullets
    #define bulletItr std::list<bullet>::const_iterator
	bulletItr bulletsBegin() { return bullets.begin(); }
	bulletItr bulletsEnd() { return bullets.end(); }
	bulletItr destroyBullet(bulletItr itr) { return bullets.erase(itr); }
};