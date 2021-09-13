#include "enemy.h"
#include "game.h"

SoLoud::Wav Enemy::damageAudio;
SoLoud::Wav Enemy::deathAudio;

Enemy::Enemy(Game &game, VoxelRenderer &renderer) : Character(game, renderer) {}

void Enemy::loadAudio()
{
	damageAudio.load("audio/enemy_hit.wav");
	deathAudio.load("audio/enemy_death.wav");
}

void Enemy::updateState(float dt)
{
	//animate
	if (game.elapsedTime - lastModelUpdate >= modelUpdateDelay)
	{
		nextModel();
	}

	//update enemy's position
	move(dt);

	//possibly fire
	if (game.elapsedTime - lastFireTime >= fireCooldown)
	{
		lastFireTime = game.elapsedTime;
		fire();
	}

	//update bullets
	moveBullets(dt);
	for (auto itr = bullets.begin(); itr != bullets.end(); itr++)
	{
		itr->trail.update(dt);
	}

	//check if tint needs to be changed
	if (game.elapsedTime - lastDamaged >= tintDuration)
		tintColor = glm::vec3(1.0f, 1.0f, 1.0f);
}

void Enemy::takeDamage()
{
	if (state != ALIVE)
		return;
	hp--;
	game.audioEngine.play(damageAudio);
	lastDamaged = game.elapsedTime;
	tintColor = glm::vec3(1.0f, 0.0f, 0.0f); //give enemy a red tint

	if (hp == 0)
	{
		game.audioEngine.play(deathAudio);
		state = DYING;
		modelUpdateDelay = 0.2f;
		modelIndex = 0;
	}
}