#include "enemy.h"

//FORWARD DECLARATIONS
class AudioPlayer;

class Enemy1 : public Enemy {
public:
	Enemy1(Game& game, VoxelRenderer& renderer);

	static void loadModels();

	void fire();
	void move(float dt);

	//audio
	static AudioPlayer shootAudio;
	static AudioPlayer damageAudio;
};