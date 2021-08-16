#include "enemy.h"

//FORWARD DECLARATIONS
class AudioPlayer;

class Enemy3 : public Enemy {
private:
	glm::vec3 bulletDir = glm::vec3(0.0f, 0.0f, 1.0f);
	const float longFireCooldown  = 3.2f;
	const float shortFireCooldown = 0.05f;
	int bulletCount = 0;
public:
	Enemy3(Game& game, VoxelRenderer& renderer);

	static void loadModels();

	void updateState(float dt);
	void fire();
	void move(float dt);

	//audio
	static AudioPlayer shootAudio;
};