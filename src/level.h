#pragma once
#include <vector>
#include <list>
#include <glm/glm.hpp>
#include "gameObject.h"

//FORWARD DECLARATIONS
class VoxelRenderer;
class Game;
class VoxelModel;
class Player;
class Character;
class Skybox;
class Enemy;
class Island;
class Boss;
struct Pickup;

/*contains data relating to enivronment and enemies, and methods for collision detection*/
class Level {
private:
	VoxelRenderer&           renderer;
	Game&                    game;
	Skybox*                  skybox;
	std::list<Island>        islands;
	std::vector<Enemy*>      enemies;
	Pickup*                  healthPickup = nullptr;
	Pickup*                  powerup      = nullptr;
	Boss*                    boss         = nullptr;

	const float levelSize = 100.0f;        //1/2 of side length of the cube that makes up the play area
	float enemySpawnDelay = 5.5f;          //time between enemy spawns
	static const int numIslands = 5;       //total amount of unique islands that can possibly spawn (excluding first island)

public:
	Level(VoxelRenderer& renderer, Game& game);
	~Level();
	void updateState(float dt);

	//lighting
	glm::vec3 lightPos = glm::vec3(42.0f, 17.0f, -35.0f);
	glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);

	//enemies
	int enemyLevel = 1;  //determines which kind of enemies to spawn
	float enemySpawnTimer = 0.0f;    //keeps track of when to spawn an enemy
	void updateEnemies(float dt);

	//pickups
	float pickupTimer    = 0.0f;
	float pickupCooldown = 10.0f;
	float pickupSpeed    = 8.0f;
	void updatePickups(float dt);
	Pickup* spawnPickup();

	//difficulty
	float lastDifficultyUpdate = 0.0f;
	float difficultyIncrement = 30.0f;
	void updateDifficulty(float dt);

	//collisions
	static glm::vec3 checkCollisionAABB(const GameObject& one, const GameObject& two);
	glm::vec3 checkPlayerLevelCollision(Player& player);
	void checkPlayerBulletCollision(Player& player);
	void checkBulletEnemyCollisions(Player& player);
	void checkBulletBossCollisions(Player& player);
	void checkPlayerPickupCollision(Player& player);
	bool outOfBounds(GameObject& obj);

	//islands
	float islandSpeed = 3.0f;
	void addIsland();
	static void loadModels(); //load all models for the level's islands

	//boss
	float bossSpawnTime = 90.0f;

	void draw();     //draw all islands and enemies

	glm::vec3 getRandPerimeterPoint(); //get a random point along the perimeter of the level
	float getLevelSize() { return levelSize; }
};

//TYPE DEFINITIONS 
/*an "island" is a single segment of the level represented by a voxel model
this class holds the model and world position data needed for collision detection*/
class Island : public GameObject {
public:
	VoxelModel& model;
	std::vector<GameObject> Voxels;  //holds voxel position data for collision detection
	void updateVoxels();
	Island(VoxelModel& model, glm::vec3 pos);
};

/*represents a health pickup or powerup*/
struct Pickup : public GameObject {
	glm::vec3 velocity;
	float speed;

	Pickup(glm::vec3 pos, glm::vec3 direction, float speed) : GameObject(pos), velocity(direction), speed(speed) { }
};