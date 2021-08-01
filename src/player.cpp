#include "player.h"

Player::Player(VoxelModel& model, VoxelRenderer* renderer): model(model){
	this->model.renderer = renderer;
}

void Player::draw() {
	model.draw();
}