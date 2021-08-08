#include "trailGenerator.h"
#include "gameObject.h"

TrailGenerator::TrailGenerator(GameObject& object, glm::vec3 color): object(&object), color(color) {
	bulletScale = object.scale;
	for (int i = 0; i < 10; i++) {
		particles.push_back(Particle(object.pos, color));
		particles[i].life = 0.0f;
	}
}

void TrailGenerator::update(float dt) {
	updateTimer += dt;
	if (updateTimer < updateDelay) return;
	updateTimer = 0.0f;

	for (int i = 0; i < 10; i++) {
		particles[i].life -= 0.1f;
		if (particles[i].life < 0) particles[i].life = 0;
	}

	if (firstUnusedIndex() == -1) return;
	particles[firstUnusedIndex()] = Particle(object->pos, color);
}

//find index of first 'dead' particle
int TrailGenerator::firstUnusedIndex() {
	for (int i = 0; i < 10; i++) {
		if (particles[i].life <= 0.0f) return i;
	}
	return -1;
}