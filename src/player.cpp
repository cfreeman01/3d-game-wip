#include "player.h"
#include <glm/gtx/intersect.hpp>

Player::Player(VoxelModel& model, Game& game, VoxelRenderer* renderer): model(model), game(game){
	this->model.renderer = renderer;
}

void Player::draw() {
	model.draw();
	drawBullets();
}

void Player::processInput(float dt) {
	glm::vec3 fb = game.mainCamera->Position - this->model.pos; //vector from player to camera (forward/back movement)
	fb = glm::normalize(glm::vec3(fb.x, 0, fb.z));

	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 lr = glm::normalize(glm::cross(up, fb));  //vector pointing to player's right (left/right movement)

	//keyboard input
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

	//mouse1 = fire
	if (game.mouse1) {
		fire();
	}

	//rotation input from mouse
	//
	glm::mat4 modelMat = glm::mat4(1.0f);
	glm::mat4 projection = game.mainCamera->GetProjectionMatrix();
	glm::mat4 view = game.mainCamera->GetViewMatrix();
	
	//obtain midPos, the point at the middle of the player model
	modelMat = glm::translate(modelMat, model.pos);
	glm::vec3 midPos = glm::vec3(0.5f * model.scale * model.sizeX, 0.5f * model.scale * model.sizeY, 0.5f * model.scale * model.sizeZ);
	midPos = modelMat * glm::vec4(midPos, 1.0f);

	//get intersection of line pointing from the cursor with the plane: y = midPos.y
	glm::vec3 cursorPosNear = glm::unProject(glm::vec3(game.mouseX, game.Height - game.mouseY, 0.0f), view, projection, glm::vec4(0, 0, game.Width, game.Height));
	glm::vec3 cursorPosFar = glm::unProject(glm::vec3(game.mouseX, game.Height - game.mouseY, 1.0f), view, projection, glm::vec4(0, 0, game.Width, game.Height));
	float distance;
	glm::intersectRayPlane(cursorPosNear, glm::normalize(cursorPosFar - cursorPosNear), midPos, glm::vec3(0, 1, 0), distance);
	glm::vec3 cursorPos = cursorPosNear + distance * glm::normalize(cursorPosFar - cursorPosNear); //intersection point
	glm::vec3 playerToCursor = glm::normalize(cursorPos - midPos);

	//get angle between playerToCursor and <-1, 0, 0>(direction player is facing when he first spawns)
	float angle = acos(glm::dot(playerToCursor, glm::vec3(-1.0f, 0.0f, 0.0f)));

	//use cross product to check if angle should be negated
	glm::vec3 cross = glm::normalize(glm::cross(playerToCursor, glm::vec3(-1.0f, 0.0f, 0.0f)));
	if (glm::abs(cross.y - 1.0f) <= .01f) {
		angle = -angle;
	}

	//finally, rotate the player
	model.rotateY = glm::degrees(angle);
}

void Player::fire() {
	glm::mat4 modelMat = glm::mat4(1.0f);
	modelMat = glm::rotate(modelMat, glm::radians(model.rotateY), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::vec3 direction = glm::normalize(modelMat * glm::vec4(-1.0f, 0.0f, 0.0f, 1.0f)); //direction the player is facing

	modelMat = glm::mat4(1.0f);
	modelMat = glm::translate(modelMat, model.pos);
	glm::vec3 midPos = glm::vec3(0.5f * model.scale * model.sizeX, 0.5f * model.scale * model.sizeY, 0.5f * model.scale * model.sizeZ);
	midPos = modelMat * glm::vec4(midPos, 1.0f);  //middle point of the player model

	bullets.push_back(bullet(midPos, direction, model.rotateY));
}

void Player::moveBullets(float dt) {
	for (int i = 0; i < bullets.size(); i++) {
		bullets[i].pos += dt * bulletSpeed * bullets[i].direction;
	}
}

void Player::drawBullets() {
	for (int i = 0; i < bullets.size(); i++) {
		model.renderer->drawVoxel(bullets[i].pos, glm::vec3(1.0f, 0.6f, 0.0f), bulletScale, bullets[i].rotation);
	}
}