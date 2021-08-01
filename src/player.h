#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "game.h"
#include "shader.h"
#include "resource_manager.h"
#include "VoxelLoader.h"
#include "VoxelModel.h"

class Player {
public:
	Game& game;
	VoxelModel& model;

	//movement
	const float speed = 10.0f;

	//methods
	Player(VoxelModel& model, Game& game, VoxelRenderer* renderer);
	void draw();
	void processInput(float dt);
};