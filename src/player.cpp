#include "player.h"
#include <glm/gtx/intersect.hpp>
#include "game.h"
#include "gameObject.h"
#include "shader.h"
#include "resource_manager.h"
#include "VoxelLoader.h"
#include "VoxelModel.h"
#include "audioPlayer.h"
#include "level.h"
#include "camera.h"

AudioPlayer Player::shootAudio;
AudioPlayer Player::movementAudio;

std::vector<glm::vec3> bulletColors = {
	glm::vec3(1.0f, 0.2f, 0.6f),
	glm::vec3(0.4f, 0.8f, 1.0f),
	glm::vec3(1.0f, 1.0f, 0.4f)
};

void Player::loadModels() {
	VoxelLoader::loadModel("models/player_0.vox", "player_0");
	VoxelLoader::loadModel("models/player_1.vox", "player_1");
}

Player::Player(Game& game, VoxelRenderer& renderer) : Character(game, renderer) {
	speed = 10.0f;
    bulletSpeed = 18.0f;
	bulletScale = 0.5f;
	modelUpdateDelay = 0.5f;

	//models must be loaded first!
	charModels.push_back(&VoxelLoader::getModel("player_0"));
	charModels.push_back(&VoxelLoader::getModel("player_1"));
	size = charModels[0]->size;
}

void Player::updateState(float dt) {
	//update model
	if (game.elapsedTime - lastModelUpdate >= modelUpdateDelay) {
		lastModelUpdate = game.elapsedTime;
		modelIndex = (modelIndex + 1) % charModels.size();
	}

	moveBullets(dt);

	//update bullet trails
	for (auto itr = bullets.begin(); itr != bullets.end(); itr++) {
		itr->trail.update(dt);
	}
}

void Player::processInput(float dt) {
	movePlayer(dt);
	moveVertical(dt);

	if (game.mouse1 && game.elapsedTime - lastFireTime >= fireCooldown) {
		lastFireTime = game.elapsedTime;
		fire();
	}

	rotatePlayer(dt);
}

void Player::movePlayer(float dt) {
	glm::vec3 fb = game.mainCamera->Position - this->pos; //vector from player to camera (forward/back movement)
	fb = glm::normalize(glm::vec3(fb.x, 0, fb.z));

	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 lr = glm::normalize(glm::cross(up, fb));  //vector pointing to player's right (left/right movement)
	glm::vec3 movement = glm::vec3(0.0f, 0.0f, 0.0f);

	//get movement from keyboard input
	if (game.Keys[GLFW_KEY_W]) {
		movement -= speed * dt * fb;
	}
	if (game.Keys[GLFW_KEY_A]) {
		movement -= speed * dt * lr;
	}
	if (game.Keys[GLFW_KEY_S]) {
		movement += speed * dt * fb;
	}
	if (game.Keys[GLFW_KEY_D]) {
		movement += speed * dt * lr;
	}

	//now move the player along x and z axis check for collisions
	glm::vec3 displacement = glm::vec3(0.0f, 0.0f, 0.0f);

	pos.x += movement.x;
	displacement = game.currentLevel->checkPlayerCollision(*this);
	pos.x += displacement.x;

	pos.z += movement.z;
	displacement = game.currentLevel->checkPlayerCollision(*this);
	pos.z += displacement.z;
}

void Player::moveVertical(float dt) {
	if (grounded) {
		if (game.Keys[GLFW_KEY_SPACE]) { //press space to jump
			movementAudio.play("audio/player_jump.mp3");
			grounded = false;
			verticalVelocity = 1.0f;
		}
	}

	if (grounded) {  //move player down and get displacement to test if there is ground below him
		pos.y += speed * dt * verticalVelocity; 
		glm::vec3 displacement = game.currentLevel->checkPlayerCollision(*this);
		pos.y += displacement.y;
		if (displacement.y <= 0) grounded = false; //if there is no ground below him, set grounded to false
	}

	if (!grounded) {
		pos.y += speed * dt * verticalVelocity; //move player vertically and then test for a collision
		game.mainCamera->moveVertical(speed * dt * verticalVelocity);  //move camera along with player
		verticalVelocity -= dt;
		glm::vec3 displacement = game.currentLevel->checkPlayerCollision(*this);
		pos.y += displacement.y;

		if (displacement.y > 0) {  //if the displacement from collision resolution is positive, then the player has hit the ground
			movementAudio.play("audio/player_land.mp3");
			grounded = true;
			verticalVelocity = -0.1f;
		}
		if (displacement.y < 0) verticalVelocity = 0.0f;  //if displacement is negative, player has hit a ceiling, so set verticalVelocity=0
	}
}

void Player::rotatePlayer(float dt) {

	glm::mat4 modelMat = glm::mat4(1.0f);
	glm::mat4 projection = game.mainCamera->GetProjectionMatrix();
	glm::mat4 view = game.mainCamera->GetViewMatrix();
	VoxelModel& model = *charModels[modelIndex];

	//obtain midPos, the point at the middle of the player model
	modelMat = glm::translate(modelMat, pos);
	glm::vec3 midPos = glm::vec3(0.5f * scale * model.size.x, 0.5f * scale * model.size.y, 0.5f * scale * model.size.z);
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
	rotate.y = glm::degrees(angle);
}

void Player::fire() {
	shootAudio.play("audio/gunshot.mp3");
	VoxelModel& model = *charModels[modelIndex];

	glm::mat4 modelMat = glm::mat4(1.0f);
	modelMat = glm::rotate(modelMat, glm::radians(rotate.y), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::vec3 direction = glm::normalize(modelMat * glm::vec4(-1.0f, 0.0f, 0.0f, 1.0f)); //direction the player is facing

	modelMat = glm::mat4(1.0f);
	modelMat = glm::translate(modelMat, pos);
	glm::vec3 midPos = glm::vec3(0.5f * scale * model.size.x, 0.5f * scale * model.size.y, 0.5f * scale * model.size.z);
	midPos = modelMat * glm::vec4(midPos, 1.0f);  //middle point of the player model

	glm::vec3 bulletColor = bulletColors[rand() % bulletColors.size()];
	bullets.emplace_back(midPos, direction, bulletColor, rotate.y, bulletScale, 10);
}