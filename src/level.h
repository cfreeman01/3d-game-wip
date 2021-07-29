#pragma once
#include "shader.h"

class level {
private:
	unsigned int VAO;
	Shader shader;
public:
	void drawGround();

	level(Shader& shader);
};