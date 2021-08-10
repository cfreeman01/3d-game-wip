#include "enemy.h"
#include "game.h"

Enemy::Enemy(Game& game, VoxelRenderer& renderer) : Character(game, renderer) {}

void Enemy::updateState(float dt) {
	//animate
	if (game.elapsedTime - lastModelUpdate >= modelUpdateDelay) {
		lastModelUpdate = game.elapsedTime;
		modelIndex = (modelIndex + 1) % charModels.size();
	}

	//update enemy's position
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
}