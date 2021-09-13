#include "enemy1.h"
#include "game.h"
#include "player.h"
#include "level.h"
#include "VoxelLoader.h"
#include "VoxelModel.h"

SoLoud::Wav Enemy1::shootAudio;

void Enemy1::loadModels()
{
	VoxelLoader::loadModel("models/enemy1/0.vox", "enemy1_0");
	VoxelLoader::loadModel("models/enemy1/1.vox", "enemy1_1");

	VoxelLoader::loadModel("models/enemy1/death0.vox", "enemy1_death0");
	VoxelLoader::loadModel("models/enemy1/death1.vox", "enemy1_death1");
	VoxelLoader::loadModel("models/enemy1/death2.vox", "enemy1_death2");
	VoxelLoader::loadModel("models/enemy1/death3.vox", "enemy1_death3");
	VoxelLoader::loadModel("models/enemy1/death4.vox", "enemy1_death4");
}

void Enemy1::loadAudio()
{
	shootAudio.load("audio/enemy1_gunshot.wav");
}

Enemy1::Enemy1(Game &game, VoxelRenderer &renderer) : Enemy(game, renderer)
{
	//set properties
	speed = 6.0f;
	bulletSpeed = 20.0f;
	bulletScale = 0.5f;
	fireCooldown = 2.0f;
	modelUpdateDelay = 0.5f;
	scale = 0.125f;

	//set models (must be loaded first)
	charModels.push_back(&VoxelLoader::getModel("enemy1_0"));
	charModels.push_back(&VoxelLoader::getModel("enemy1_1"));

	deathModels.push_back(&VoxelLoader::getModel("enemy1_death0"));
	deathModels.push_back(&VoxelLoader::getModel("enemy1_death1"));
	deathModels.push_back(&VoxelLoader::getModel("enemy1_death2"));
	deathModels.push_back(&VoxelLoader::getModel("enemy1_death3"));
	deathModels.push_back(&VoxelLoader::getModel("enemy1_death4"));

	//initial position
	pos = game.currentLevel->getRandPerimeterPoint() * 0.8f;
	pos.y = game.player->pos.y;
}

void Enemy1::move(float dt)
{
	//move toward player
	glm::vec3 diff = game.player->pos - this->pos;
	pos.y += 2 * speed * dt * glm::normalize(diff).y;
	if (glm::sqrt((diff.x * diff.x) + (diff.z * diff.z)) > 16.0f)
	{
		pos.x += speed * dt * glm::normalize(diff).x;
		pos.z += speed * dt * glm::normalize(diff).z;
	}

	//rotate to face player
	float angle = acos(glm::dot(glm::normalize(diff), glm::vec3(-1.0f, 0.0f, 0.0f)));
	glm::vec3 cross = glm::normalize(glm::cross(glm::normalize(diff), glm::vec3(-1.0f, 0.0f, 0.0f)));
	if (glm::abs(cross.y - 1.0f) <= .01f)
	{
		angle = -angle;
	}
	rotate.y = glm::degrees(angle);
}

void Enemy1::fire()
{
	VoxelModel *model;
	if (state == ALIVE)
		model = charModels[modelIndex];
	else
		model = deathModels[modelIndex];

	glm::vec3 diff = game.player->pos - this->pos;

	//get middle point of enemy model
	glm::mat4 modelMat = glm::mat4(1.0f);
	modelMat = glm::translate(modelMat, pos);
	glm::vec3 midPos = glm::vec3(0.5f * scale * model->getSize().x, 0.5f * scale * model->getSize().y, 0.5f * scale * model->getSize().z);
	midPos = modelMat * glm::vec4(midPos, 1.0f); //middle point of the player model

	bullets.emplace_back(midPos, glm::normalize(diff), glm::vec3(1.0f, 0.6f, 0.2f), rotate.y, bulletScale, 3);
	
	game.audioEngine.play(shootAudio);
}