#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "gameObject.h"

//FORWARD DECLARATIONS
class VoxelRenderer;

struct Voxel: public GameObject {
	/*holds voxel data that is not passed to the shader*/
	int colorIndex = 0;
};

struct VoxelRenderData {
	/*holds voxel data that is passed to the shader*/
	float x, y, z;   //offset from model position
	float R, G, B;   //color
};

class VoxelModel: public GameObject {
public:
	std::vector<Voxel> Voxels;      //voxel position data
	VoxelRenderData*   vRenderData; //render data stored as dynamically allocated array
	VoxelRenderer*     renderer;

	void draw();
	void updateVoxels();

	VoxelModel(VoxelRenderer* renderer = nullptr);
};
