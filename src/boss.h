#include "character.h"

//FORWARD DECLARATIONS
class AudioPlayer;

class Boss : public Character {
private:
	//firing/bullets
	void fire();

	//movement
	glm::vec3 velocity;
	void move(float dt);

	//audio
	static AudioPlayer shootAudio;
	static AudioPlayer damageAudio;
public:
	Boss(Game& game, VoxelRenderer& renderer);
	static void loadModels();
	void updateState(float dt);
	void takeDamage();
};