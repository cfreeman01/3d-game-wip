#include "level.h"
#include "resource_manager.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

level::level(Shader& shader) : shader(shader) {
	float groundVertices[] = {
		-200.0f, 0.0f, -200.0f,
		-200.0f, 0.0f, 200.0f,
		 200.0f, 0.0f, 0.0f,
		-200.0f, 0.0f, 200.0f,
		 200.0f, 0.0f, 200.0f,
		 200.0f, 0.0f, 0.0f
	};
	float groundTexCoords[] = {
	 0.0f, 0.0f,
	 0.0f, 1.0f,
	 1.0f, 0.0f,
	 0.0f, 1.0f,
	 1.0f, 1.0f,
	 1.0f, 0.0f
	};
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int pointsVBO;
	glGenBuffers(1, &pointsVBO);
	glBindBuffer(GL_ARRAY_BUFFER, pointsVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(groundVertices), groundVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	unsigned int textureVBO;
	glGenBuffers(1, &textureVBO);
	glBindBuffer(GL_ARRAY_BUFFER, textureVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(groundTexCoords), groundTexCoords, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);

	shader.Use();
	shader.SetInteger("voxTexture", 0);
	glBindVertexArray(0);
}

void level::drawGround() {
	shader.Use();
	ResourceManager::GetTexture("empty").Bind();
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);

	view = glm::translate(view, glm::vec3(0.0f, -30.0f, -100.0f));   //camera view
	view = glm::rotate(view, glm::radians(25.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	view = glm::rotate(view, glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	shader.SetMatrix4("model", model);
	shader.SetMatrix4("view", view);
	shader.SetVector3f("voxColor", glm::vec3(0.08f, 0.69f, 0.102f));

	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

