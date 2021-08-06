#include "level.h"
#include "player.h"
#include "character.h"
#include "VoxelRenderer.h"
#include "VoxelModel.h"
#include "VoxelLoader.h"
#include "VoxelRenderer.h"
#include "gameObject.h"
#include "shader.h"

Level::Level(VoxelRenderer* renderer, Game& game) : renderer(renderer), game(game) {
	islands.push_back(VoxelLoader::loadModel("models/first_island.vox", "first_island", renderer));
	islands[0].pos = glm::vec3(-(float)islands[0].size.x / 2, -(float)islands[0].size.y / 2, -(float)islands[0].size.z / 2);
	renderer->shader.SetVector3f("lightPos", lightPos);
	renderer->shader.SetVector3f("lightColor", lightColor);
}

void Level::drawIslands() {
	for (int i = 0; i < islands.size(); i++) {
		islands[i].draw();
	}
}

//COLLISIONS--------
//detect a collision between two axis-aligned bounding boxes and return the displacement of one if a collision occured
glm::vec3 Level::checkCollisionAABB(GameObject& one, GameObject& two) {
	glm::vec3 displacement = glm::vec3(0.0f, 0.0f, 0.0f);
	//x-axis
	bool collisionX = one.pos.x + (one.size.x * one.scale) > two.pos.x && two.pos.x + (two.size.x * two.scale) > one.pos.x;
	if (collisionX) {
		if (one.pos.x <= two.pos.x) displacement.x = two.pos.x - (one.pos.x + (one.size.x * one.scale));
		else displacement.x = (two.pos.x + (two.size.x * two.scale)) - one.pos.x;
	}

	//y-axis
	bool collisionY = one.pos.y + (one.size.y * one.scale) > two.pos.y && two.pos.y + (two.size.y * two.scale) > one.pos.y;
	if (collisionY) {
		if (one.pos.y <= two.pos.y) displacement.y = two.pos.y - (one.pos.y + (one.size.y * one.scale));
		else displacement.y = (two.pos.y + (two.size.y * two.scale)) - one.pos.y;
	}

	//z-axis
	bool collisionZ = one.pos.z + (one.size.z * one.scale) > two.pos.z && two.pos.z + (two.size.z * two.scale) > one.pos.z;
	if (collisionZ) {
		if (one.pos.z <= two.pos.z) displacement.z = two.pos.z - (one.pos.z + (one.size.z * one.scale));
		else displacement.z = (two.pos.z + (two.size.z * two.scale)) - one.pos.z;
	}

	if (collisionX && collisionY && collisionZ) {
		return displacement;
	}
	else return glm::vec3(0.0f, 0.0f, 0.0f);
}

glm::vec3 Level::checkPlayerCollision(Player& player) {
	glm::vec3 displacement = glm::vec3(0.0f, 0.0f, 0.0f);

	for (int i = 0; i < islands.size(); i++) {
		for (int j = 0; j < islands[i].Voxels.size(); j++) {
			displacement = checkCollisionAABB(player.model, islands[i].Voxels[j]);
			if (displacement != glm::vec3(0.0f, 0.0f, 0.0f)) {
				break;
			}
		}
		if (displacement != glm::vec3(0.0f, 0.0f, 0.0f)) break;
	}

	return displacement;
}
//------------------


