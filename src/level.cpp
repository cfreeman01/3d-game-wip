#include "level.h"
#include "game.h"
#include "audioPlayer.h"
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
#include "boss.h"
#include <iostream>
#include <cmath>

Island::Island(VoxelModel& model, glm::vec3 pos) : model(model) {
	Voxels = std::vector<GameObject>(model.getNumVoxels());
	size = model.getSize();
	this->pos = pos;
}


void Island::updateVoxels() {
	for (int i = 0; i < Voxels.size(); i++) {
		Voxels[i].rotate = rotate;
		Voxels[i].scale = scale;

		glm::vec3 localVoxPos = model.getVoxPos(i);
		Voxels[i].pos = pos + glm::vec3(localVoxPos.x * scale, localVoxPos.y * scale, localVoxPos.z * scale);
	}
}

void Level::loadModels() {
	for (int i = 0; i <= numIslands; i++) {
		std::string filename = "models/islands/" + std::to_string(i) + ".vox";
		std::string islandName = "island_" + std::to_string(i);
		VoxelLoader::loadModel(filename.c_str(), islandName.c_str());
	}

	VoxelLoader::loadModel("models/heart.vox", "heart");
	VoxelLoader::loadModel("models/star.vox", "star");
}

Level::Level(VoxelRenderer& renderer, Game& game) : renderer(renderer), game(game) {
	//initialize islands
	VoxelModel& island_0 = VoxelLoader::getModel("island_0");
	islands.push_back(Island(VoxelLoader::getModel("island_0"), 
		glm::vec3(-island_0.getSize().x / 2, -island_0.getSize().y / 2, -island_0.getSize().z / 2)));

	//initialize lighting
	renderer.setLightPos(lightPos);
	renderer.setLightColor(lightColor);

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
	if (healthPickup != nullptr) delete healthPickup;
	if (powerup != nullptr) delete powerup;
	if (boss != nullptr) delete boss;
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
	if (powerup != nullptr)
		renderer.drawVoxelModel(VoxelLoader::getModel("star"), *powerup);

	//draw boss
	if (boss != nullptr)
		boss->draw();
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
		if (enemies[i]->getState() == Character::DEAD) {
			delete enemies[i];
			enemies.erase(enemies.begin() + i);
			enemySpawnTimer -= 2.0f;  //turn back spawn timer as reward for killing an enemy
			i--;
		}
		else
			enemies[i]->updateState(dt);
	}

	//update or remove boss
	if (boss != nullptr) {
		boss->updateState(dt);
		if (boss->getState() == Character::DEAD) {
			delete boss;
			boss = nullptr;
			bossSpawnTime = game.elapsedTime + 60.0f;
		}
	}

	//possibly spawn new enemy
	if (enemySpawnTimer >= enemySpawnDelay && enemies.size() < 5 && boss == nullptr) {
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
			if (rand() % 2 == 0)
				healthPickup = spawnPickup();
			else
				powerup = spawnPickup();
		}
		else pickupTimer -= 5.0f;
	}

	//move pickups
	if (healthPickup != nullptr) {
		healthPickup->pos += dt * healthPickup->speed * healthPickup->velocity;
		healthPickup->pos.y = game.player->pos.y;
		healthPickup->rotate.y += (360 * dt);
	}
	if (powerup != nullptr) {
		powerup->pos += dt * powerup->speed * powerup->velocity;
		powerup->pos.y = game.player->pos.y;
		powerup->rotate.y += (360 * dt);
	}

	//check if pickups are out of bounds
	if (healthPickup != nullptr) {
		if (outOfBounds(*healthPickup)) {
			delete healthPickup;
			healthPickup = nullptr;
		}
	}
	if (powerup != nullptr) {
		if (outOfBounds(*powerup)) {
			delete powerup;
			powerup = nullptr;
		}
	}
}

Pickup* Level::spawnPickup() {
	pickupTimer = 0;
	glm::vec3 newPos = getRandPerimeterPoint();
	newPos.y = game.player->pos.y;
	glm::vec3 newVelocity;
	if (newPos.x < 0) newVelocity = glm::vec3(1, 0, 0);
	else newVelocity = glm::vec3(-1, 0, 0);
	Pickup* pickup = new Pickup(newPos, newVelocity, pickupSpeed);
	pickup->scale = 0.13f;
	return pickup;
}

void Level::updateDifficulty(float dt) {
	if (game.elapsedTime - lastDifficultyUpdate >= difficultyIncrement) {
		lastDifficultyUpdate = game.elapsedTime;

		if (enemyLevel < 3) enemyLevel++;  //allow more enemy types to spawn

		islandSpeed += 0.3f;

		if(enemySpawnDelay > 3.0f) enemySpawnDelay -= 0.2f;

		if (game.elapsedTime >= bossSpawnTime && boss == nullptr) {  //possibly spawn a boss
			boss = new Boss(game, renderer);
			boss->pos = glm::vec3(40.0f, 0.0f, 40.0f);
		}
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

bool Level::outOfBounds(GameObject& obj) {
	if (obj.pos.x >= levelSize || obj.pos.x <= -levelSize
		|| obj.pos.y >= levelSize || obj.pos.y <= -levelSize
		|| obj.pos.z >= levelSize || obj.pos.z <= -levelSize) {
		return true; //obj is out of bounds
	}
	return false;
}

//COLLISIONS--------
//detect a collision between two axis-aligned bounding boxes and return the displacement of one if a collision occured
glm::vec3 Level::checkCollisionAABB(const GameObject& one, const GameObject& two) {
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

	//check collision with island
	for (int j = 0; j < currentIsland->Voxels.size(); j++) {
		displacement = checkCollisionAABB(player, currentIsland->Voxels[j]);
		if (displacement != glm::vec3(0.0f, 0.0f, 0.0f)) {
			break;
		}
	}

	return displacement;
}

//check if player collides with pickups
void Level::checkPlayerPickupCollision(Player& player) {
	if (healthPickup != nullptr) {
		if (checkCollisionAABB(player, *healthPickup) != glm::vec3(0, 0, 0)) {
			player.incHP();
			delete healthPickup;
			healthPickup = nullptr;
			game.gameAudio.play("audio/hp_gain.mp3");
		}
	}

	if (powerup != nullptr) {
		if (checkCollisionAABB(player, *powerup) != glm::vec3(0, 0, 0)) {
			player.powerUp();
			delete powerup;
			powerup = nullptr;
			game.gameAudio.play("audio/powerup.mp3");
		}
	}
}

//check if player's bullets collide with enemies
void Level::checkBulletEnemyCollisions(Player& player) {
	for (auto bullet = player.bulletsBegin(); bullet != player.bulletsEnd(); bullet++) {
		for (int i = 0; i < enemies.size(); i++) {

			if (enemies[i]->getState() == Character::ALIVE) {
				if (checkCollisionAABB(*bullet, *enemies[i]) != glm::vec3(0, 0, 0)) {
					enemies[i]->takeDamage();
					bullet = player.destroyBullet(bullet);
					if (bullet == player.bulletsBegin()) return;
					bullet--;
					break;
				}
			}

		}
	}
	if (boss != nullptr) checkBulletBossCollisions(player);
}

//check if player's bullets collide with boss
void Level::checkBulletBossCollisions(Player& player) {
	for (auto bullet = player.bulletsBegin(); bullet != player.bulletsEnd(); bullet++) {
		if (checkCollisionAABB(*bullet, *boss) != glm::vec3(0, 0, 0)) {
			boss->takeDamage();
			bullet = player.destroyBullet(bullet);
			if (bullet == player.bulletsBegin()) return;
			bullet--;
			break;
		}
	}
}

//check if enemy's or boss' bullets collide with player
void Level::checkPlayerBulletCollision(Player& player) {
	for (int i = 0; i < enemies.size(); i++) {
		for (auto bullet = enemies[i]->bulletsBegin(); bullet != enemies[i]->bulletsEnd(); bullet++) {
			if (checkCollisionAABB(player, *bullet) != glm::vec3(0, 0, 0)) {
				enemies[i]->destroyBullet(bullet);
				player.takeDamage();
				return;
			}
		}
	}

	if (boss != nullptr) {
		for (auto bullet = boss->bulletsBegin(); bullet != boss->bulletsEnd(); bullet++) {
			if (checkCollisionAABB(player, *bullet) != glm::vec3(0, 0, 0)) {
				boss->destroyBullet(bullet);
				player.takeDamage();
				return;
			}
		}
	}
}