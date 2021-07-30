#pragma once
#include "shader.h"
#include "game.h"
#include "VoxelRenderer.h"

class levelRenderer {
private:
	unsigned int VAO;
	Shader shader;
	Game& game;
	VoxelRenderer& voxelRenderer;

	//lighting
	glm::vec3 lightPos = glm::vec3(42.0f, 17.0f, -35.0f);
	glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
public:
	void drawLightSource();

	levelRenderer(Shader& shader, Game& game, VoxelRenderer& voxelRenderer);
};