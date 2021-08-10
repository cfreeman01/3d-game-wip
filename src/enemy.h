#pragma once
#include "character.h"

/*base class for all enemies*/
class Enemy : public Character {
public:
	Enemy(Game& game, VoxelRenderer& renderer);

	void updateState(float dt);
	virtual void fire()=0;
	virtual void move(float dt)=0;
};