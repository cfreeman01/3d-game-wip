#include "enemy.h"
#include "game.h"
#include "audioPlayer.h"
AudioPlayer Enemy::damageAudio;


Enemy::Enemy(Game& game, VoxelRenderer& renderer) : Character(game, renderer) {}

void Enemy::updateState(float dt) {
	//animate
	if (game.elapsedTime - lastModelUpdate >= modelUpdateDelay) {
		nextModel();
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

	//check if tint needs to be changed
	if (game.elapsedTime - lastDamaged >= tintDuration)
		tintColor = glm::vec3(1.0f, 1.0f, 1.0f);
}

void Enemy::takeDamage() {
	if (cState != ALIVE) return;
	hp--;
	lastDamaged = game.elapsedTime;
	tintColor = glm::vec3(1.0f, 0.0f, 0.0f);  //give enemy a red tint

	if (hp == 0) {
		damageAudio.play("audio/enemy_death.mp3");
		cState = DYING;
		modelUpdateDelay = 0.2f;
		modelIndex = 0;
	}
	else
		damageAudio.play("audio/enemy_hit.mp3");
}