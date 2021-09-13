#include "character.h"
#include "soloud.h"
#include "soloud_wav.h"

class Boss : public Character {
private:
	//firing/bullets
	void fire();

	//movement
	glm::vec3 velocity;
	void move(float dt);
	
	//audio
	SoLoud::Wav shootAudio;
	SoLoud::Wav damageAudio;
	SoLoud::Wav deathAudio;

public:
	Boss(Game& game, VoxelRenderer& renderer);
	static void loadModels();
	void updateState(float dt);
	void takeDamage();
};