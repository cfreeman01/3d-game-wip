#include "boss.h"
#include "VoxelLoader.h"
#include "audioPlayer.h"
#include "game.h"
#include "player.h"
#include "level.h"
#include "VoxelModel.h"

AudioPlayer Boss::shootAudio;
AudioPlayer Boss::damageAudio;

//possible colors for bullets fired by the boss
std::vector<glm::vec3> bossBulletColors = {
	glm::vec3(0.87f, 0.0f, 0.0f),
	glm::vec3(0.2f, 0.6f, 0.6f),
	glm::vec3(0.2f, 1.0f, 0.2f),
	glm::vec3(0.4f, 0.0f, 1.0f),
	glm::vec3(1.0f, 0.8f, 0.0f),
	glm::vec3(1.0f, 0.4f, 0.0f)
};

//direction vectors for bullets fired by the boss
//each vector is a point on a square perimeter, to permit the bullets to fire in a square pattern
std::vector<glm::vec3> bossBulletDirections = {
	//+x
	glm::vec3(0.5f, 0.0f, -0.5f),
	glm::vec3(0.5f, 0.0f, -0.4f),
	glm::vec3(0.5f, 0.0f, -0.3f),
	glm::vec3(0.5f, 0.0f, -0.2f),
	glm::vec3(0.5f, 0.0f, -0.1f),
	glm::vec3(0.5f, 0.0f, 0.0f),
	glm::vec3(0.5f, 0.0f, 0.1f),
	glm::vec3(0.5f, 0.0f, 0.2f),
	glm::vec3(0.5f, 0.0f, 0.3f),
	glm::vec3(0.5f, 0.0f, 0.4f),
	glm::vec3(0.5f, 0.0f, 0.5f),
	//-x
	glm::vec3(-0.5f, 0.0f, -0.5f),
	glm::vec3(-0.5f, 0.0f, -0.4f),
	glm::vec3(-0.5f, 0.0f, -0.3f),
	glm::vec3(-0.5f, 0.0f, -0.2f),
	glm::vec3(-0.5f, 0.0f, -0.1f),
	glm::vec3(-0.5f, 0.0f, 0.0f),
	glm::vec3(-0.5f, 0.0f, 0.1f),
	glm::vec3(-0.5f, 0.0f, 0.2f),
	glm::vec3(-0.5f, 0.0f, 0.3f),
	glm::vec3(-0.5f, 0.0f, 0.4f),
	glm::vec3(-0.5f, 0.0f, 0.5f),
	//+z
	glm::vec3(-0.5f, 0.0f, 0.5f),
	glm::vec3(-0.4f, 0.0f, 0.5f),
	glm::vec3(-0.3f, 0.0f, 0.5f),
	glm::vec3(-0.2f, 0.0f, 0.5f),
	glm::vec3(-0.1f, 0.0f, 0.5f),
	glm::vec3(0.0f, 0.0f, 0.5f),
	glm::vec3(0.1f, 0.0f, 0.5f),
	glm::vec3(0.2f, 0.0f, 0.5f),
	glm::vec3(0.3f, 0.0f, 0.5f),
	glm::vec3(0.4f, 0.0f, 0.5f),
	glm::vec3(0.5f, 0.0f, 0.5f),
	//-z
	glm::vec3(-0.5f, 0.0f, -0.5f),
	glm::vec3(-0.4f, 0.0f, -0.5f),
	glm::vec3(-0.3f, 0.0f, -0.5f),
	glm::vec3(-0.2f, 0.0f, -0.5f),
	glm::vec3(-0.1f, 0.0f, -0.5f),
	glm::vec3(0.0f, 0.0f, -0.5f),
	glm::vec3(0.1f, 0.0f, -0.5f),
	glm::vec3(0.2f, 0.0f, -0.5f),
	glm::vec3(0.3f, 0.0f, -0.5f),
	glm::vec3(0.4f, 0.0f, -0.5f),
	glm::vec3(0.5f, 0.0f, -0.5f),
};

void Boss::loadModels() {
	VoxelLoader::loadModel("models/boss/0.vox", "boss_0");
	VoxelLoader::loadModel("models/boss/1.vox", "boss_1");
	VoxelLoader::loadModel("models/boss/2.vox", "boss_2");

	VoxelLoader::loadModel("models/boss/death_0.vox", "boss_death_0");
	VoxelLoader::loadModel("models/boss/death_1.vox", "boss_death_1");
	VoxelLoader::loadModel("models/boss/death_2.vox", "boss_death_2");
	VoxelLoader::loadModel("models/boss/death_3.vox", "boss_death_3");
	VoxelLoader::loadModel("models/boss/death_4.vox", "boss_death_4");
	VoxelLoader::loadModel("models/boss/death_5.vox", "boss_death_5");
	VoxelLoader::loadModel("models/boss/death_6.vox", "boss_death_6");
	VoxelLoader::loadModel("models/boss/death_7.vox", "boss_death_7");
}

Boss::Boss(Game& game, VoxelRenderer& renderer): Character(game,renderer) {
	//set properties
	speed = 10.0f;
	hp = 30;
	velocity = glm::normalize(glm::vec3(-1, 0, -1));
	bulletSpeed = 23.0f;
	bulletScale = 0.5f;
	fireCooldown = 2.5f;
	modelUpdateDelay = 0.25f;
	scale = 0.2f;

	//set models (must be loaded first)
	charModels.push_back(&VoxelLoader::getModel("boss_0"));
	charModels.push_back(&VoxelLoader::getModel("boss_1"));
	charModels.push_back(&VoxelLoader::getModel("boss_2"));

	deathModels.push_back(&VoxelLoader::getModel("boss_death_0"));
	deathModels.push_back(&VoxelLoader::getModel("boss_death_1"));
	deathModels.push_back(&VoxelLoader::getModel("boss_death_2"));
	deathModels.push_back(&VoxelLoader::getModel("boss_death_3"));
	deathModels.push_back(&VoxelLoader::getModel("boss_death_4"));
	deathModels.push_back(&VoxelLoader::getModel("boss_death_5"));
	deathModels.push_back(&VoxelLoader::getModel("boss_death_6"));
	deathModels.push_back(&VoxelLoader::getModel("boss_death_7"));
}

void Boss::updateState(float dt) {
	//animate
	if (game.elapsedTime - lastModelUpdate >= modelUpdateDelay) {
		nextModel();
	}

	//update position
	move(dt);

	//possibly fire
	if (game.elapsedTime - lastFireTime >= fireCooldown) {
		lastFireTime = game.elapsedTime;
		fire();
	}

	//update bullets
	moveBullets(dt);
	for (auto itr = bullets.begin(); itr != bullets.end(); itr++) {
		itr->trail.update(dt);
	}

	//check if tint needs to be changed
	if (game.elapsedTime - lastDamaged >= tintDuration)
		tintColor = glm::vec3(1.0f, 1.0f, 1.0f);
}

void Boss::move(float dt) {
	glm::vec3 diff = game.player->pos - this->pos;
	float arenaSize = game.currentLevel->getLevelSize() / 2;

	pos += speed * dt * velocity;                      //move in x-z plane
	pos.y += 2 * speed * dt * glm::normalize(diff).y;  //move toward player's y position

	//bounce the boss off sides of the arena
	if (pos.x >= arenaSize || pos.x <= -arenaSize) {
		velocity.x = -velocity.x;
	}
	if (pos.z >= arenaSize || pos.z <= -arenaSize) {
		velocity.z = -velocity.z;
	}

	//rotate
	rotate.x += 60 * dt;
	rotate.y += 60 * dt;
}

void Boss::takeDamage() {
	if (state != ALIVE) return;
	hp--;
	lastDamaged = game.elapsedTime;
	tintColor = glm::vec3(1.0f, 0.0f, 0.0f);  //give enemy a red tint

	if (hp == 0) {
		damageAudio.play("audio/boss_death.mp3");
		state = DYING;
		modelUpdateDelay = 0.2f;
		modelIndex = 0;
	}
	else
		damageAudio.play("audio/boss_hit.mp3");
}

void Boss::fire() {
	shootAudio.play("audio/boss_fire.mp3");
	VoxelModel* model;
	if (state == ALIVE)
		model = charModels[modelIndex];
	else
		model = deathModels[modelIndex];

	//get bottom-middle point of the boss models
	glm::mat4 modelMat = glm::mat4(1.0f);
	modelMat = glm::translate(modelMat, pos);
	glm::vec3 midPos = glm::vec3(0.5f * scale * model->getSize().x, 0.0f, 0.5f * scale * model->getSize().z);
	midPos = modelMat * glm::vec4(midPos, 1.0f);  //middle point of the player model

	for (int i = 0; i < bossBulletDirections.size(); i++) {
		bullets.emplace_back(midPos, bossBulletDirections[i], bossBulletColors[rand() % bossBulletColors.size()], rotate.y, bulletScale, 2);
	}
}