#pragma once
#include "shader.h"
#include "VoxelModel.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//contains data and methods to render voxels
class VoxelRenderer {
public:
	unsigned int VAO;
	Shader shader;

	void initRenderData();
	void drawVoxel(Voxel& voxel);

	VoxelRenderer(Shader& shader);
};