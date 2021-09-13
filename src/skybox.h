#pragma once
#include <vector>
#include <string>

//FORWARD DECLARATIONS
class Shader;
class Game;

class Skybox
{
private:
	Game &game;
	unsigned int VAO;
	unsigned int textureID;
	float size;

	unsigned int loadTextures(std::vector<std::string> textures);
	unsigned int initRenderData();

public:
	Skybox(Game &game, std::vector<std::string> textures, float size);
	void draw();
};