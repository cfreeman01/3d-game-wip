#pragma once
#include <vector>
#include <glm/glm.hpp>

//FORWARD DECLARATIONS
class GameObject;

struct Particle {
	float x, y, z;
	float R, G, B;
	float life = 1.0f;

	Particle(glm::vec3 pos, glm::vec3 color){
		x = pos.x;
		y = pos.y;
		z = pos.z;
		R = color.x;
		G = color.y;
		B = color.z;
	}
};

/*generates a particle trail behind a moving object*/
class TrailGenerator {
public:
	GameObject* object;  
	glm::vec3 color;
	std::vector<Particle> particles;
	float updateTimer = 0.0f;
	float updateDelay = 0.05f;
	int numParticles;

	TrailGenerator() {}
	TrailGenerator(GameObject* object, glm::vec3 color, int numParticles);
	void update(float dt);
};