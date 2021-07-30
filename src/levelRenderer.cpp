#include "levelRenderer.h"
#include "resource_manager.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

levelRenderer::levelRenderer(Shader& shader, Game& game, VoxelRenderer& voxelRenderer) : shader(shader), game(game), voxelRenderer(voxelRenderer) {
	float groundVertices[] = {
		//vertices                //normals
		-500.0f, 0.0f, -500.0f,   0.0f, 1.0f, 0.0f,
		-500.0f, 0.0f, 500.0f,    0.0f, 1.0f, 0.0f,
		 500.0f, 0.0f, -500.0f,   0.0f, 1.0f, 0.0f,
		-500.0f, 0.0f, 500.0f,    0.0f, 1.0f, 0.0f,
		 500.0f, 0.0f, 500.0f,    0.0f, 1.0f, 0.0f,
		 500.0f, 0.0f, -500.0f,   0.0f, 1.0f, 0.0f,
	};
	float groundTexCoords[] = {
		0.0f,    0.0f,
		0.0f,    250.0f,
		250.0f,  0.0f,
		0.0f,    250.0f,
		250.0f,  250.0f,
		250.0f,  0.0f
	};

	ResourceManager::LoadTexture("textures/ground.png", true, "ground");

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int pointsVBO;
	glGenBuffers(1, &pointsVBO);
	glBindBuffer(GL_ARRAY_BUFFER, pointsVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(groundVertices), groundVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	unsigned int textureVBO;
	glGenBuffers(1, &textureVBO);
	glBindBuffer(GL_ARRAY_BUFFER, textureVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(groundTexCoords), groundTexCoords, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);

	unsigned int normalsVBO;
	glGenBuffers(1, &normalsVBO);
	glBindBuffer(GL_ARRAY_BUFFER, normalsVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(groundVertices), groundVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);

	shader.Use();
	shader.SetInteger("voxTexture", 0);
	shader.SetVector3f("lightColor", lightColor);
	shader.SetVector3f("lightPos", lightPos);
	glBindVertexArray(0);
}

void levelRenderer::drawLightSource() {
	voxelRenderer.drawVoxel(lightPos, glm::vec3(1.0f, 1.0f, 1.0f), 2.0f);
}

