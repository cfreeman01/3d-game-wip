#include "level.h"
#include "player.h"
#include "character.h"
#include "VoxelRenderer.h"
#include "VoxelModel.h"
#include "VoxelLoader.h"
#include "VoxelRenderer.h"
#include "gameObject.h"
#include "shader.h"
#include "skybox.h"

Island::Island(VoxelModel& model) : model(model) {
	Voxels = std::vector<GameObject>(model.numVoxels);
	size = model.size;
}

void Island::updateVoxels() {
	for (int i = 0; i < Voxels.size(); i++) {
		Voxels[i].rotate = rotate;
		Voxels[i].scale = scale;
		Voxels[i].pos = pos + glm::vec3(model.vRenderData[i].x * scale, model.vRenderData[i].y * scale, model.vRenderData[i].z * scale);
	}
}

Level::Level(VoxelRenderer& renderer, Game& game) : renderer(renderer), game(game) {
	//initialize islands
	islands.push_back(Island(VoxelLoader::loadModel("models/first_island.vox", "first_island")));
	islands[0].pos = glm::vec3(-(float)islands[0].model.size.x / 2, -(float)islands[0].model.size.y / 2, -(float)islands[0].model.size.z / 2);

	//initialize lighting
	renderer.shader.SetVector3f("lightPos", lightPos);
	renderer.shader.SetVector3f("lightColor", lightColor);

	//initialize skybox
	std::vector<std::string> skyboxTextures = {
			"textures/skybox_right.png",
			"textures/skybox_left.png",
			"textures/skybox_top.png",
			"textures/skybox_bottom.png",
			"textures/skybox_front.png",
			"textures/skybox_back.png"
	};
	skybox = new Skybox(game, skyboxTextures, levelSize);
}

void Level::draw() {
	//draw skybox
	skybox->draw();

	//draw islands
	for (int i = 0; i < islands.size(); i++) {
		renderer.drawVoxelModel(islands[i].model, islands[i]);
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
		islands[i].updateVoxels();
		for (int j = 0; j < islands[i].Voxels.size(); j++) {
			displacement = checkCollisionAABB(player, islands[i].Voxels[j]);
			if (displacement != glm::vec3(0.0f, 0.0f, 0.0f)) {
				break;
			}
		}
		if (displacement != glm::vec3(0.0f, 0.0f, 0.0f)) break;
	}

	return displacement;
}

void Level::checkBulletsCollisions(Character& character) {
	for (auto itr = character.bullets.begin(); itr != character.bullets.end(); itr++) {
		for (int i = 0; i < islands.size(); i++) {
			islands[i].updateVoxels();
			for (int j = 0; j < islands[i].Voxels.size(); j++) {
				GameObject currentVoxel = islands[i].Voxels[j];
				if (checkCollisionAABB(*itr, currentVoxel) != glm::vec3(0, 0, 0)) {
					itr = character.bullets.erase(itr);
					if (itr == character.bullets.begin()) return;
					itr--;
				}

			}
		}
	}
}