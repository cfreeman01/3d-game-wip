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
#include "enemy3.h"
#include <iostream>
#include <cmath>

Island::Island(VoxelModel& model, glm::vec3 pos) : model(model) {
	Voxels = std::vector<GameObject>(model.numVoxels);
	size = model.size;
	this->pos = pos;
}

void Level::loadModels() {
	for (int i = 0; i <= numIslands; i++) {
		std::string filename = "models/islands/" + std::to_string(i) + ".vox";
		std::string islandName = "island_" + std::to_string(i);
		VoxelLoader::loadModel(filename.c_str(), islandName.c_str());
	}

	VoxelLoader::loadModel("models/heart.vox", "heart");
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
	VoxelModel& island_0 = VoxelLoader::getModel("island_0");
	islands.push_back(Island(VoxelLoader::getModel("island_0"), 
		glm::vec3(-island_0.size.x / 2, -island_0.size.y / 2, -island_0.size.z / 2)));

	//initialize lighting
	renderer.shader.SetVector3f("lightPos", lightPos, true);
	renderer.shader.SetVector3f("lightColor", lightColor, true);

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
	for (auto itr = islands.begin(); itr != islands.end(); itr++) {
		renderer.drawVoxelModel(itr->model, *itr);
	}

	//draw enemies
	for (int i = 0; i < enemies.size(); i++) {
		enemies[i]->draw();
	}

	//draw pickups
	if (healthPickup != nullptr)
		renderer.drawVoxelModel(VoxelLoader::getModel("heart"), *healthPickup);
}

void Level::updateState(float dt) {
	updateEnemies(dt);
	updateDifficulty(dt);
	updatePickups(dt);

	//move islands
	for (auto itr = islands.begin(); itr != islands.end(); itr++) {
		itr->pos.z -= islandSpeed * dt;
	}

	//possibly add new island
	Island frontIsland = *(--islands.end());
	float frontPos = frontIsland.pos.z + (frontIsland.size.z * frontIsland.scale);
	if (frontPos <= levelSize) addIsland();

	//possibly delete old island
	float backPos = islands.begin()->pos.z;
	if (backPos <= -(levelSize)) {
		islands.pop_front();
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
		if (enemyType == 3) {
			enemies.push_back(new Enemy3(game, renderer));
		}
	}
}

void Level::updatePickups(float dt) {
	pickupTimer += dt;

	//possibly spawn new pickup
	if (pickupTimer >= pickupCooldown) {

		if (rand() % 2 == 0) {
			//spawn health pickup
			pickupTimer = 0;
			glm::vec3 newPos = getRandPerimeterPoint();
			glm::vec3 newVelocity;
			if (newPos.x < 0) newVelocity = glm::vec3(1, 0, 0);
			else newVelocity = glm::vec3(-1, 0, 0);
			healthPickup = new Pickup(newPos, newVelocity, 5.0f);
			healthPickup->scale = 0.125f;
		}

		else pickupTimer -= 5.0f;
	}

	//move pickups
	if (healthPickup != nullptr)
		healthPickup->pos += dt * healthPickup->speed * healthPickup->velocity;
}

void Level::updateDifficulty(float dt) {
	if (game.elapsedTime - lastDifficultyUpdate >= 30.0f) {
		lastDifficultyUpdate = game.elapsedTime;
		if (enemyLevel < 3) enemyLevel++;
	}
}

void Level::addIsland() {
	int newIslandNum = (rand() % numIslands) + 1;
	std::string newIslandName = "island_" + std::to_string(newIslandNum);

	Island frontIsland = *(--islands.end());

	glm::vec3 midPos = frontIsland.pos;  //get front island's middle point (along the x-axis)
	midPos.x += ((frontIsland.size.x * frontIsland.scale) / 2);

	glm::vec3 newPos = midPos;
	newPos.z += frontIsland.size.z * frontIsland.scale;

	islands.push_back(Island(VoxelLoader::getModel(newIslandName), newPos));

	//adjust x-position so that middle of new island aligns with middle of previous island
	islands.back().pos.x -= (islands.back().size.x * islands.back().scale) / 2;
}

/*returns a random point on the perimeter of the level where enemies can spawn*/
glm::vec3 Level::getRandPerimeterPoint() {
	glm::vec3 result = glm::vec3(0,0,0);
	int side = rand() % 2;

	if (side == 0) {
		result.x = -(levelSize / 2);
		result.z = fmod(rand(), levelSize) - (levelSize / 2);
	}
	else if (side == 1) {
		result.x = levelSize / 2;
		result.z = fmod(rand(), levelSize) - (levelSize / 2);
	}

	return result;
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

	//find island that player is currently on
	Island* currentIsland = nullptr;
	for (auto island = islands.begin(); island != islands.end(); island++) {
		if (checkCollisionAABB(player, *island) != glm::vec3(0, 0, 0)) {
			currentIsland = &(*island);
			break;
		}
	}
	if (currentIsland == nullptr) return glm::vec3(0, 0, 0);

	currentIsland->updateVoxels();

	for (int j = 0; j < currentIsland->Voxels.size(); j++) {
		displacement = checkCollisionAABB(player, currentIsland->Voxels[j]);
		if (displacement != glm::vec3(0.0f, 0.0f, 0.0f)) {
			break;
		}
	}

	return displacement;
}

//check if player's bullets collide with enemies
void Level::checkBulletEnemyCollisions(Player& player) {
	for (auto bullet = player.bullets.begin(); bullet != player.bullets.end(); bullet++) {
		for (int i = 0; i < enemies.size(); i++) {

			if (enemies[i]->cState == ALIVE) {
				if (checkCollisionAABB(*bullet, *enemies[i]) != glm::vec3(0, 0, 0)) {
					enemies[i]->takeDamage();
					bullet = player.bullets.erase(bullet);
					if (bullet == player.bullets.begin()) return;
					bullet--;
					break;
				}
			}

		}
	}
}

//check if enemy's bullets collide with player
void Level::checkPlayerBulletCollision(Player& player) {
	for (int i = 0; i < enemies.size(); i++) {
		for (auto bullet = enemies[i]->bullets.begin(); bullet != enemies[i]->bullets.end(); bullet++) {
			if (checkCollisionAABB(player, *bullet) != glm::vec3(0, 0, 0)) {
				enemies[i]->bullets.erase(bullet);
				player.takeDamage();
				return;
			}
		}
	}
}