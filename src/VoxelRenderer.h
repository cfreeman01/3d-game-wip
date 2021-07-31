#pragma once
#include "shader.h"
#include "texture.h"
#include "game.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Voxel;     //forward declarations, full declarations in VoxelModel.h
class VoxelModel;

//contains data and methods to render voxels
class VoxelRenderer {
public:
	unsigned int  VAO;
	Shader&       shader;
	Game&         game;

	void initRenderData();
	void drawVoxel(Voxel& voxel, VoxelModel& model);              //draw a voxel that is part of a model
	void drawVoxel(glm::vec3 pos, glm::vec3 color, float scale);  //draw a standalone voxel

	VoxelRenderer(Shader& shader, Game& game);
};