#include "enemy.h"

//FORWARD DECLARATIONS
class AudioPlayer;

class Enemy1 : public Enemy {
private:
	void fire();
	void move(float dt);
	static AudioPlayer shootAudio;

public:
	Enemy1(Game& game, VoxelRenderer& renderer);
	static void loadModels();
};