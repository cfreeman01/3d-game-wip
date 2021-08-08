#include "character.h"
#include "game.h"
#include "gameObject.h"
#include "shader.h"
#include "resource_manager.h"
#include "VoxelLoader.h"
#include "VoxelModel.h"
#include "VoxelRenderer.h"
#include "level.h"

void Character::draw() {
	renderer.drawVoxelModel(*charModels[modelIndex],*this);
	drawBullets();
}

void Character::moveBullets(float dt) {
	for (int i = 0; i < bullets.size(); i++) {
		bullets[i].pos.x += dt * bulletSpeed * bullets[i].direction.x;
		bullets[i].pos.y += dt * bulletSpeed * bullets[i].direction.y;
		bullets[i].pos.z += dt * bulletSpeed * bullets[i].direction.z;

		//check if out of bounds
		if (bullets[i].pos.x >= game.currentLevel->levelSize / 2 || bullets[i].pos.x <= -game.currentLevel->levelSize / 2
			|| bullets[i].pos.y >= game.currentLevel->levelSize / 2 || bullets[i].pos.y <= -game.currentLevel->levelSize / 2
			|| bullets[i].pos.z >= game.currentLevel->levelSize / 2 || bullets[i].pos.z <= -game.currentLevel->levelSize / 2) {
			bullets.erase(bullets.begin() + i);
			i--;
		}
	}

	//check if bullets collide with level
	game.currentLevel->checkBulletsCollisions(*this);
}

void Character::drawBullets() {
	if (bullets.empty()) return;
	renderer.drawBullets(*this);
	for (int i = 0; i < bullets.size(); i++) {
		renderer.drawTrail(bullets[i].trail);
	}
}