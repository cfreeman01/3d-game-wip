#include "character.h"
#include "game.h"
#include "gameObject.h"
#include "shader.h"
#include "resource_manager.h"
#include "VoxelLoader.h"
#include "VoxelModel.h"
#include "VoxelRenderer.h"
#include "level.h"
#include <iostream>

float bulletTime = 0.0f;
float playerTime = 0.0f;
float lastTime = 0.0f;

void Character::draw()
{
	if (state == ALIVE)
		renderer.drawVoxelModel(*charModels[modelIndex], *this);
	else if (state == DYING)
		renderer.drawVoxelModel(*deathModels[modelIndex], *this);

	drawBullets();
}

void Character::moveBullets(float dt)
{
	for (auto itr = bullets.begin(); itr != bullets.end(); itr++)
	{
		itr->pos.x += dt * bulletSpeed * itr->direction.x;
		itr->pos.y += dt * bulletSpeed * itr->direction.y;
		itr->pos.z += dt * bulletSpeed * itr->direction.z;

		//check if out of bounds
		if (game.currentLevel->outOfBounds(*itr))
		{
			itr = bullets.erase(itr);
			if (itr == bullets.begin())
				break;
			itr--;
		}
	}
}

void Character::nextModel()
{ //animate the character by switching the voxel model
	lastModelUpdate = game.elapsedTime;

	if (state == ALIVE)
	{
		modelIndex = (modelIndex + 1) % charModels.size();
		size = charModels[modelIndex]->getSize();
	}

	if (state == DYING)
	{
		if (modelIndex == deathModels.size() - 1)
		{
			state = DEAD;
			return;
		}
		modelIndex = (modelIndex + 1) % deathModels.size();
		size = deathModels[modelIndex]->getSize();
	}
}

void Character::drawBullets()
{
	if (bullets.empty())
		return;
		
	renderer.drawBullets(*this);
	renderer.drawTrails(*this);
}