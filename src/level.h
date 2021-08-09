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

class Island : public GameObject {
	/*holds position data for an island and a reference to the model data*/
public:
	VoxelModel& model;
	std::vector<GameObject> Voxels;  //holds voxel position data for collision detection
	void updateVoxels();
	Island(VoxelModel& model);
};

class Level {
private:
	VoxelRenderer&           renderer;
	Game&                    game;
	Skybox*                  skybox;
	std::vector<Island>      islands;
public:
	const float levelSize = 100.0f;  //side length of the cube that makes up the play area

	//lighting
	glm::vec3 lightPos = glm::vec3(42.0f, 17.0f, -35.0f);
	glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);

	//collisions
	static glm::vec3 checkCollisionAABB(GameObject& one, GameObject& two);
	glm::vec3 checkPlayerCollision(Player& player);
	void checkBulletsCollisions(Character& character);

	Level(VoxelRenderer& renderer, Game& game);
	void draw();
};