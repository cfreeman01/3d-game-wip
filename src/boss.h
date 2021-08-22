#include "character.h"

//FORWARD DECLARATIONS
class AudioPlayer;

class Boss : public Character {
private:
	void takeDamage();
	void fire();
	void move(float dt);
	static AudioPlayer shootAudio;

public:
	Boss(Game& game, VoxelRenderer& renderer);
	static void loadModels();
	void updateState(float dt);
};