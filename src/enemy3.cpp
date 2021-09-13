#include "enemy3.h"
#include "game.h"
#include "player.h"
#include "level.h"
#include "VoxelLoader.h"
#include "VoxelModel.h"

SoLoud::Wav Enemy3::shootAudio;

void Enemy3::loadModels() {
	VoxelLoader::loadModel("models/enemy3/0.vox", "enemy3_0");
	VoxelLoader::loadModel("models/enemy3/1.vox", "enemy3_1");
	VoxelLoader::loadModel("models/enemy3/2.vox", "enemy3_2");
	VoxelLoader::loadModel("models/enemy3/3.vox", "enemy3_3");
	VoxelLoader::loadModel("models/enemy3/4.vox", "enemy3_4");

	VoxelLoader::loadModel("models/enemy3/death0.vox", "enemy3_death0");
	VoxelLoader::loadModel("models/enemy3/death1.vox", "enemy3_death1");
	VoxelLoader::loadModel("models/enemy3/death2.vox", "enemy3_death2");
	VoxelLoader::loadModel("models/enemy3/death3.vox", "enemy3_death3");
	VoxelLoader::loadModel("models/enemy3/death4.vox", "enemy3_death4");
}

void Enemy3::loadAudio(){
	shootAudio.load("audio/enemy3_gunshot.wav");
}

Enemy3::Enemy3(Game& game, VoxelRenderer& renderer) : Enemy(game, renderer) {
	//set properties
	speed = 6.0f;
	bulletSpeed = 20.0f;
	bulletScale = 0.5f;
	fireCooldown = longFireCooldown;
	modelUpdateDelay = 0.2f;
	scale = 0.14f;

	//set models (must be loaded first)
	charModels.push_back(&VoxelLoader::getModel("enemy3_0"));
	charModels.push_back(&VoxelLoader::getModel("enemy3_1"));
	charModels.push_back(&VoxelLoader::getModel("enemy3_2"));
	charModels.push_back(&VoxelLoader::getModel("enemy3_3"));
	charModels.push_back(&VoxelLoader::getModel("enemy3_4"));

	deathModels.push_back(&VoxelLoader::getModel("enemy3_death0"));
	deathModels.push_back(&VoxelLoader::getModel("enemy3_death1"));
	deathModels.push_back(&VoxelLoader::getModel("enemy3_death2"));
	deathModels.push_back(&VoxelLoader::getModel("enemy3_death3"));
	deathModels.push_back(&VoxelLoader::getModel("enemy3_death4"));

	//initial position
	pos = game.currentLevel->getRandPerimeterPoint() * 0.8f;
	pos.y = game.player->pos.y;
}

void Enemy3::updateState(float dt) {
	//animate
	if (game.elapsedTime - lastModelUpdate >= modelUpdateDelay) {
		nextModel();
	}

	//update enemy's position
	move(dt);

	//possibly fire
	if (game.elapsedTime - lastFireTime >= fireCooldown) {
		lastFireTime = game.elapsedTime;

		if (glm::abs(fireCooldown - longFireCooldown) <= 0.01f) {  //if fireCooldown is at its higher value
			fireCooldown = shortFireCooldown;            //set it to lower value (to fire repeatedly)
			bulletCount = 0;
			game.audioEngine.play(shootAudio);
		}

		if (glm::abs(fireCooldown - shortFireCooldown) <= 0.01f) { //if fireCooldown is at its lower value (enemy is firing)
			fire();
			bulletCount++;
			if (bulletCount == 15) fireCooldown = longFireCooldown;
		}
	}

	//update bullets
	moveBullets(dt);
	for (auto itr = bullets.begin(); itr != bullets.end(); itr++) {
		itr->trail.update(dt);
	}

	//update bulletDir
	glm::mat4 rot = glm::mat4(1.0f);
	rot = glm::rotate(rot, glm::radians(360 * dt), glm::vec3(0,1,0));
	bulletDir = rot * glm::vec4(bulletDir, 1.0f);

	//check if tint needs to be changed
	if (game.elapsedTime - lastDamaged >= tintDuration)
		tintColor = glm::vec3(1.0f, 1.0f, 1.0f);
}

void Enemy3::move(float dt) {
	//move toward player
	glm::vec3 diff = game.player->pos - this->pos;
	pos.y += 2 * speed * dt * glm::normalize(diff).y;
	if (glm::sqrt((diff.x * diff.x) + (diff.z * diff.z)) > 16.0f) {
		pos.x += speed * dt * glm::normalize(diff).x;
		pos.z += speed * dt * glm::normalize(diff).z;
	}

	//rotate to face player
	float angle = acos(glm::dot(glm::normalize(diff), glm::vec3(-1.0f, 0.0f, 0.0f)));
	glm::vec3 cross = glm::normalize(glm::cross(glm::normalize(diff), glm::vec3(-1.0f, 0.0f, 0.0f)));
	if (glm::abs(cross.y - 1.0f) <= .01f) {
		angle = -angle;
	}
	rotate.y = glm::degrees(angle);
}

void Enemy3::fire() {
	VoxelModel* model;
	if (state == ALIVE)
		model = charModels[modelIndex];
	else
		model = deathModels[modelIndex];

	//get middle point of enemy model
	glm::mat4 modelMat = glm::mat4(1.0f);
	modelMat = glm::translate(modelMat, pos);
	glm::vec3 midPos = glm::vec3(0.5f * scale * model->getSize().x, 0.5f * scale * model->getSize().y, 0.5f * scale * model->getSize().z);
	midPos = modelMat * glm::vec4(midPos, 1.0f);  //middle point of the player model

	bullets.emplace_back(midPos, glm::normalize(bulletDir), glm::vec3(0.8f, 0.2f, 0.2f), rotate.y, bulletScale, 3);
}