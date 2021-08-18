#include "enemy.h"

//FORWARD DECLARATIONS
class AudioPlayer;

class Enemy2 : public Enemy {
private:
	void fire();
	void move(float dt);
	static AudioPlayer shootAudio;

public:
	Enemy2(Game& game, VoxelRenderer& renderer);
	static void loadModels();
};