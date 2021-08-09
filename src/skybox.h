#pragma once
#include <vector>
#include <string>

//FORWARD DECLARATIONS
class Shader;
class Game;

class Skybox {
public:
	Game& game;
	unsigned int VAO;
	unsigned int textureID;
	float size;
	Skybox(Game& game, std::vector<std::string> textures, float size);
	void draw();
private:
	unsigned int loadTextures(std::vector<std::string> textures);
	unsigned int initRenderData();
};