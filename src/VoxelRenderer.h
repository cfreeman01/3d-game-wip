#pragma once
#include "shader.h"
#include "texture.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Voxel;     //forward declarations, full declarations in VoxelModel.h
class VoxelModel;

//contains data and methods to render voxels
class VoxelRenderer {
public:
	unsigned int VAO;
	Shader shader;

	void initRenderData();
	void drawVoxel(Voxel& voxel, VoxelModel& model);

	VoxelRenderer(Shader& shader);
};