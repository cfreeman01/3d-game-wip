#pragma once
#include "character.h"

//FORWARD DECLARATIONS
class AudioPlayer;

/*base class for all enemies*/
class Enemy : public Character {
public:
	Enemy(Game& game, VoxelRenderer& renderer);

	void updateState(float dt);
	void takeDamage();
	virtual void fire()=0;
	virtual void move(float dt)=0;

	static AudioPlayer damageAudio;
};