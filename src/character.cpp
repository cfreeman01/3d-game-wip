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
	model.draw();
	drawBullets();
}

void Character::moveBullets(float dt) {
	for (int i = 0; i < bullets.size(); i++) {
		bullets[i].x += dt * bulletSpeed * bullets[i].direction.x;
		bullets[i].y += dt * bulletSpeed * bullets[i].direction.y;
		bullets[i].z += dt * bulletSpeed * bullets[i].direction.z;

		//check if out of bounds
		if (bullets[i].x >= game.currentLevel->levelSize / 2 || bullets[i].x <= -game.currentLevel->levelSize / 2
			|| bullets[i].y >= game.currentLevel->levelSize / 2 || bullets[i].y <= -game.currentLevel->levelSize / 2
			|| bullets[i].z >= game.currentLevel->levelSize / 2 || bullets[i].z <= -game.currentLevel->levelSize / 2) {
			bullets.erase(bullets.begin() + i);
			i--;
		}
	}
}

void Character::drawBullets() {
	if (bullets.empty()) return;
	model.renderer->drawBullets(*this);
}