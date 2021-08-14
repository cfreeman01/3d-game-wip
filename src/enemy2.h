#include "enemy.h"

//FORWARD DECLARATIONS
class AudioPlayer;

class Enemy2 : public Enemy {
public:
	Enemy2(Game& game, VoxelRenderer& renderer);

	static void loadModels();

	void fire();
	void move(float dt);

	//audio
	static AudioPlayer shootAudio;
};