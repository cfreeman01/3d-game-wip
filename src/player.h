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
	VoxelModel& model;

	Player(VoxelModel& model, VoxelRenderer* renderer);
	void draw();
};