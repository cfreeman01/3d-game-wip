#pragma once
#include <vector>
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

/*holds position data for an island and a reference to the model data*/
class Island : public GameObject {
public:
	VoxelModel& model;
	std::vector<GameObject> Voxels;  //holds voxel position data for collision detection
	void updateVoxels();
	Island(VoxelModel& model);
};

/*contains data relating to enivronment and enemies, and methods for collision detection*/
class Level {
private:
	VoxelRenderer&           renderer;
	Game&                    game;
	Skybox*                  skybox;
	std::vector<Island>      islands;
	std::vector<Enemy*>      enemies;
public:
	//consts
	const float levelSize       = 100.0f;  //side length of the cube that makes up the play area
	const float enemySpawnDelay = 4.0f;    //time between enemy spawns

	//lighting
	glm::vec3 lightPos = glm::vec3(42.0f, 17.0f, -35.0f);
	glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);

	//enemies
	int enemyLevel = 1;  //determines which kind of enemies to spawn
	float enemySpawnTimer = 0.0f;    //keeps track of when to spawn an enemy
	void updateEnemies(float dt);

	//difficulty
	float lastDifficultyUpdate = 0.0f;
	void updateDifficulty(float dt);

	//collisions
	static glm::vec3 checkCollisionAABB(GameObject& one, GameObject& two);
	glm::vec3 checkPlayerLevelCollision(Player& player);
	void checkPlayerBulletCollision(Player& player);
	void checkBulletLevelCollisions(Character& character);
	void checkBulletEnemyCollisions(Player& player);

	Level(VoxelRenderer& renderer, Game& game);
	~Level();
	static void loadModels();
	void draw();
};