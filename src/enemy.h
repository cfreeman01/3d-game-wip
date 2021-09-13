#pragma once
#include "character.h"
#include "soloud.h"
#include "soloud_wav.h"

/*base class for all enemies*/
class Enemy : public Character
{
protected:
	virtual void fire() = 0;
	virtual void move(float dt) = 0;

	//audio
	static SoLoud::Wav damageAudio;
	static SoLoud::Wav deathAudio;

public:
	Enemy(Game &game, VoxelRenderer &renderer);

	static void loadAudio();
	void updateState(float dt);
	void takeDamage();
};