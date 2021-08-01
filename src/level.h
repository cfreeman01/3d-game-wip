#pragma once
#include "VoxelRenderer.h"
#include "VoxelModel.h"
#include "VoxelLoader.h"
#include <vector>

class Level {
private:
	VoxelRenderer&           renderer;
	Game&                    game;
	std::vector<VoxelModel>  islands;
public:
	//lighting
	glm::vec3 lightPos = glm::vec3(42.0f, 17.0f, -35.0f);
	glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);

	Level(VoxelRenderer& renderer, Game& game);
	void drawIslands();
};