#include "enemy2.h"
#include "game.h"
#include "player.h"
#include "level.h"
#include "VoxelLoader.h"
#include "VoxelModel.h"

SoLoud::Wav Enemy2::shootAudio;

void Enemy2::loadModels() {
	VoxelLoader::loadModel("models/enemy2/0.vox", "enemy2_0");
	VoxelLoader::loadModel("models/enemy2/1.vox", "enemy2_1");
	VoxelLoader::loadModel("models/enemy2/2.vox", "enemy2_2");
	VoxelLoader::loadModel("models/enemy2/-1.vox", "enemy2_-1");
	VoxelLoader::loadModel("models/enemy2/-2.vox", "enemy2_-2");

	VoxelLoader::loadModel("models/enemy2/death0.vox", "enemy2_death0");
	VoxelLoader::loadModel("models/enemy2/death1.vox", "enemy2_death1");
	VoxelLoader::loadModel("models/enemy2/death2.vox", "enemy2_death2");
	VoxelLoader::loadModel("models/enemy2/death3.vox", "enemy2_death3");
}

void Enemy2::loadAudio(){
	shootAudio.load("audio/enemy2_gunshot.wav");
}

Enemy2::Enemy2(Game& game, VoxelRenderer& renderer) : Enemy(game, renderer) {
	//set properties
	speed = 5.0f;
	bulletSpeed = 20.0f;
	bulletScale = 0.55f;
	fireCooldown = 2.0f;
	modelUpdateDelay = 0.2f;
	scale = 0.15f;

	//set models (must be loaded first)
	charModels.push_back(&VoxelLoader::getModel("enemy2_0"));
	charModels.push_back(&VoxelLoader::getModel("enemy2_1"));
	charModels.push_back(&VoxelLoader::getModel("enemy2_2"));
	charModels.push_back(&VoxelLoader::getModel("enemy2_1"));
	charModels.push_back(&VoxelLoader::getModel("enemy2_0"));
	charModels.push_back(&VoxelLoader::getModel("enemy2_-1"));
	charModels.push_back(&VoxelLoader::getModel("enemy2_-2"));
	charModels.push_back(&VoxelLoader::getModel("enemy2_-1"));

	deathModels.push_back(&VoxelLoader::getModel("enemy2_death0"));
	deathModels.push_back(&VoxelLoader::getModel("enemy2_death1"));
	deathModels.push_back(&VoxelLoader::getModel("enemy2_death2"));
	deathModels.push_back(&VoxelLoader::getModel("enemy2_death3"));

	//initial position
	pos = game.currentLevel->getRandPerimeterPoint() * 0.8f;
	pos.y = game.player->pos.y;
}

void Enemy2::move(float dt) {
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

void Enemy2::fire() {
	VoxelModel* model;
	if (state == ALIVE)
		model = charModels[modelIndex];
	else
		model = deathModels[modelIndex];

	glm::vec3 diff = game.player->pos - this->pos;

	//get middle point of enemy model
	glm::mat4 modelMat = glm::mat4(1.0f);
	modelMat = glm::translate(modelMat, pos);
	glm::vec3 midPos = glm::vec3(0.5f * scale * model->getSize().x, 0.5f * scale * model->getSize().y, 0.5f * scale * model->getSize().z);
	midPos = modelMat * glm::vec4(midPos, 1.0f);  //middle point of the player model

	//fire bullet directly toward the player
	bullets.emplace_back(midPos, glm::normalize(diff), glm::vec3(0.2f, 0.8f, 0.6f), rotate.y, bulletScale, 3);

	//fire two more bullets at an angle from the first one
	modelMat = glm::mat4(1.0f);
	modelMat = glm::translate(modelMat, -midPos);
	modelMat = glm::rotate(modelMat, glm::radians(-25.0f), glm::vec3(0, 1, 0));
	modelMat = glm::translate(modelMat, midPos);
	glm::vec3 newDir = glm::normalize(modelMat * glm::vec4(diff, 1.0f));
	bullets.emplace_back(midPos, newDir, glm::vec3(0.2f, 0.8f, 0.6f), rotate.y, bulletScale, 3);

	modelMat = glm::mat4(1.0f);
	modelMat = glm::translate(modelMat, -midPos);
	modelMat = glm::rotate(modelMat, glm::radians(25.0f), glm::vec3(0, 1, 0));
	modelMat = glm::translate(modelMat, midPos);
	newDir = glm::normalize(modelMat * glm::vec4(diff, 1.0f));
	bullets.emplace_back(midPos, newDir, glm::vec3(0.2f, 0.8f, 0.6f), rotate.y, bulletScale, 3);

	game.audioEngine.play(shootAudio);
}