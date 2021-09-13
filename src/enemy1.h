#include "enemy.h"
#include "soloud.h"
#include "soloud_wav.h"

class Enemy1 : public Enemy
{
private:
	void fire();
	void move(float dt);

	//audio
	static SoLoud::Wav shootAudio;

public:
	Enemy1(Game &game, VoxelRenderer &renderer);

	static void loadModels();
	static void loadAudio();
};