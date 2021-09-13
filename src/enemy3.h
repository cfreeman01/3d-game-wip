#include "enemy.h"
#include "soloud.h"
#include "soloud_wav.h"

class Enemy3 : public Enemy {
private:
	void fire();
	void move(float dt);

	//audio
	static SoLoud::Wav shootAudio;

	/*properites used to manage unique firing pattern*/
	glm::vec3 bulletDir = glm::vec3(0.0f, 0.0f, 1.0f);
	const float longFireCooldown  = 3.2f;
	const float shortFireCooldown = 0.05f;
	int bulletCount = 0;

public:
	Enemy3(Game& game, VoxelRenderer& renderer);
	static void loadModels();
	static void loadAudio();
	void updateState(float dt);
};