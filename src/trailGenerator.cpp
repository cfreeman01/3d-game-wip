#include "trailGenerator.h"
#include "gameObject.h"

TrailGenerator::TrailGenerator(GameObject* object, glm::vec3 color): object(object), color(color) {
	particles.push_back(Particle(object->pos, color));
}

void TrailGenerator::update(float dt) {
	updateTimer += dt;
	if (updateTimer < updateDelay) return;
	updateTimer = 0.0f;

	//decrease life of particles and remove dead particles
	for (int i = 0; i < particles.size(); i++) {
		particles[i].life -= 0.1f;
		if (particles[i].life <= 0.0f) {
			particles.erase(particles.begin() + i);
			i--;
		}
	}

	//iff number of particles is less than numParticles, add a new particle
	if (particles.size() < 10) {
		particles.emplace_back(object->pos, color);
	}
}