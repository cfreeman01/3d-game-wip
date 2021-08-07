#pragma once

//FORWARD DECLARATIONS
class Game;
class GameObject;
struct Voxel;
class VoxelModel;
class Character;
class Shader;

//contains data and methods to render voxels
class VoxelRenderer {
public:
	unsigned int  VAO;
	Shader&       shader;
	Game&         game;

	void initRenderData();
	void drawVoxelModel(VoxelModel& model, GameObject& object);    //draw a voxel model
	void drawBullets(Character& character);    //draw a set of bullets for a character

	VoxelRenderer(Shader& shader, Game& game);
};