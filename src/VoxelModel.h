#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "gameObject.h"

//FORWARD DECLARATIONS
class VoxelRenderer;

struct VoxelRenderData {
	/*holds voxel data that is passed to the shader*/
	float x, y, z;   //offset from model position
	float R, G, B;   //color
	int colorIndex;
};

class VoxelModel{
public:
	int numVoxels;
	glm::vec3 size;
	VoxelRenderData*   vRenderData; //render data stored as dynamically allocated array

	VoxelModel();
};
