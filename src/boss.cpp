#include "boss.h"
#include "VoxelLoader.h"
#include "audioPlayer.h"
#include "game.h"

AudioPlayer Boss::shootAudio;

void Boss::loadModels() {
	VoxelLoader::loadModel("models/boss/0.vox", "boss_0");
	VoxelLoader::loadModel("models/boss/1.vox", "boss_1");
	VoxelLoader::loadModel("models/boss/2.vox", "boss_2");
}

Boss::Boss(Game& game, VoxelRenderer& renderer): Character(game,renderer) {
	//set properties
	speed = 6.0f;
	bulletSpeed = 20.0f;
	bulletScale = 0.5f;
	fireCooldown = 2.0f;
	modelUpdateDelay = 0.25f;
	scale = 0.13f;

	//set models (must be loaded first)
	charModels.push_back(&VoxelLoader::getModel("boss_0"));
	charModels.push_back(&VoxelLoader::getModel("boss_1"));
	charModels.push_back(&VoxelLoader::getModel("boss_2"));
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

void Boss::fire() {

}

void Boss::move(float dt) {
	//move
	if (pos.z >= 25.0f) pos.z -= speed * dt;

	//rotate
	rotate.x += 60 * dt;
	rotate.y += 60 * dt;
}

void Boss::takeDamage() {

}