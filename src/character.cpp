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
	for (auto itr = bullets.begin(); itr != bullets.end(); itr++) {
		itr->pos.x += dt * bulletSpeed * itr->direction.x;
		itr->pos.y += dt * bulletSpeed * itr->direction.y;
		itr->pos.z += dt * bulletSpeed * itr->direction.z;

		//check if out of bounds
		if (itr->pos.x >= game.currentLevel->levelSize / 2 || itr->pos.x <= -game.currentLevel->levelSize / 2
			|| itr->pos.y >= game.currentLevel->levelSize / 2 || itr->pos.y <= -game.currentLevel->levelSize / 2
			|| itr->pos.z >= game.currentLevel->levelSize / 2 || itr->pos.z <= -game.currentLevel->levelSize / 2) {
			itr = bullets.erase(itr);
			if (itr == bullets.begin()) break;
			itr--;
		}
	}

	//check if bullets collide with level
	game.currentLevel->checkBulletsCollisions(*this);
}

void Character::drawBullets() {
	if (bullets.empty()) return;
	renderer.drawBullets(*this);
	renderer.drawTrails(*this);
}