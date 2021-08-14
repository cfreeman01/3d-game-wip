#include "level.h"
#include "game.h"
#include "player.h"
#include "character.h"
#include "VoxelRenderer.h"
#include "VoxelModel.h"
#include "VoxelLoader.h"
#include "VoxelRenderer.h"
#include "gameObject.h"
#include "shader.h"
#include "skybox.h"
#include "enemy.h"
#include "enemy1.h"
#include "enemy2.h"
#include <iostream>

Island::Island(VoxelModel& model) : model(model) {
	Voxels = std::vector<GameObject>(model.numVoxels);
	size = model.size;
}

void Level::loadModels() {
	VoxelLoader::loadModel("models/islands/first_island.vox", "first_island");
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
	islands.push_back(Island(VoxelLoader::getModel("first_island")));
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

Level::~Level() {
	delete skybox;
	for (int i = 0; i < enemies.size(); i++)
		delete enemies[i];
}

void Level::draw() {
	//draw skybox
	skybox->draw();

	//draw islands
	for (int i = 0; i < islands.size(); i++) {
		renderer.drawVoxelModel(islands[i].model, islands[i]);
	}

	//draw enemies
	for (int i = 0; i < enemies.size(); i++) {
		enemies[i]->draw();
	}
}

void Level::updateEnemies(float dt) {
	enemySpawnTimer += dt;

	//update or remove existing enemies
	for (int i = 0; i < enemies.size(); i++) {
		if (enemies[i]->cState == DEAD) {
			delete enemies[i];
			enemies.erase(enemies.begin() + i);
			enemySpawnTimer -= 2.0f;  //turn back spawn timer as reward for killing an enemy
			i--;
		}
		else
			enemies[i]->updateState(dt);
	}

	//possibly spawn new enemy
	if (enemySpawnTimer >= enemySpawnDelay && enemies.size() < 5) {
		enemySpawnTimer = 0.0f;
		int enemyType = (rand() % enemyLevel) + 1;
		if (enemyType == 1) {
			enemies.push_back(new Enemy1(game, renderer));
		}
		if (enemyType == 2) {
			enemies.push_back(new Enemy2(game, renderer));
		}
	}
}

void Level::updateDifficulty(float dt) {
	if (game.elapsedTime - lastDifficultyUpdate >= 30.0f) {
		lastDifficultyUpdate = game.elapsedTime;
		if (enemyLevel < 2) enemyLevel++;
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

//check if player collides with level
glm::vec3 Level::checkPlayerLevelCollision(Player& player) {
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

//check if character's bullets collide with the level
void Level::checkBulletLevelCollisions(Character& character) {
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

//check if player's bullets collide with enemies
void Level::checkBulletEnemyCollisions(Player& player) {
	for (auto itr = player.bullets.begin(); itr != player.bullets.end(); itr++) {
		for (int i = 0; i < enemies.size(); i++) {
			if (game.Keys[GLFW_KEY_R]) {
				int x = 2;
			}
			if (enemies[i]->cState == ALIVE) {
				if (checkCollisionAABB(*itr, *enemies[i]) != glm::vec3(0, 0, 0)) {
					enemies[i]->takeDamage();
					itr = player.bullets.erase(itr);
					if (itr == player.bullets.begin()) return;
					itr--;
					break;
				}
			}
		}
	}
}

//check if enemy's bullets collide with player
void Level::checkPlayerBulletCollision(Player& player) {
	for (int i = 0; i < enemies.size(); i++) {
		for (auto b = enemies[i]->bullets.begin(); b != enemies[i]->bullets.end(); b++) {
			if (checkCollisionAABB(player, *b) != glm::vec3(0, 0, 0)) {
				enemies[i]->bullets.erase(b);
				player.takeDamage();
				return;
			}
		}
	}
}