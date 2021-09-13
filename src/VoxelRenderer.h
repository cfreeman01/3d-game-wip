#pragma once
#include "shader.h"

//FORWARD DECLARATIONS
class Game;
class GameObject;
struct Voxel;
class VoxelModel;
class Character;
class TrailGenerator;

/*contains data and methods to render voxels*/
class VoxelRenderer
{
protected:
	Game &game;
	Shader &shader;
	unsigned int VAO;
	void initRenderData();

public:
	void drawVoxelModel(VoxelModel &model, GameObject &object); //draw a voxel model
	void drawBullets(Character &character);						//draw a set of bullets for a character
	void drawTrails(Character &character);

	void setLightPos(glm::vec3 pos) { shader.SetVector3f("lightPos", pos, true); }
	void setLightColor(glm::vec3 color) { shader.SetVector3f("lightColor", color, true); }

	VoxelRenderer(Shader &shader, Game &game);
};