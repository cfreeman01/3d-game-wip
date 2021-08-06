#pragma once
#include <vector>
#include <glm/glm.hpp>

//FORWARD DECLARATIONS
class VoxelRenderer;
class Game;
class VoxelModel;
class GameObject;
class Player;

class Level {
private:
	VoxelRenderer*           renderer;
	Game&                    game;
	std::vector<VoxelModel>  islands;
public:
	const float levelSize = 100.0f;  //side length of the cube that makes up the play area

	//lighting
	glm::vec3 lightPos = glm::vec3(42.0f, 17.0f, -35.0f);
	glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);

	//collisions
	static glm::vec3 checkCollisionAABB(GameObject& one, GameObject& two);
	glm::vec3 checkPlayerCollision(Player& player);

	Level(VoxelRenderer* renderer, Game& game);
	void drawIslands();
};