#include "player.h"

Player::Player(VoxelModel& model, Game& game, VoxelRenderer* renderer): model(model), game(game){
	this->model.renderer = renderer;
}

void Player::draw() {
	model.draw();
}

void Player::processInput(float dt) {
	glm::vec3 fb = game.mainCamera->Position - this->model.pos; //vector from player to camera (forward/back movement)
	fb = glm::normalize(glm::vec3(fb.x, 0, fb.z));

	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 lr = glm::normalize(glm::cross(up, fb));  //vector pointing to player's right (left/right movement)

	if (game.Keys[GLFW_KEY_W]) {
		this->model.pos -= speed * dt * fb;
	}
	if (game.Keys[GLFW_KEY_A]) {
		this->model.pos -= speed * dt * lr;
	}
	if (game.Keys[GLFW_KEY_S]) {
		this->model.pos += speed * dt * fb;
	}
	if (game.Keys[GLFW_KEY_D]) {
		this->model.pos += speed * dt * lr;
	}
}