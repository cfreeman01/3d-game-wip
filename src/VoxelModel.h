#pragma once
#include "GameObject.h"
#include "VoxelRenderer.h"
#include <glm/glm.hpp>
#include <vector>

struct Voxel: public GameObject {
	int x, y, z;              //x,y,z position relative to model's position
	unsigned int colorIndex;  //index into model's color palette
	Voxel(unsigned int x, unsigned int y, unsigned int z, unsigned int colorIndex) : GameObject(), x(x), y(y), z(z), colorIndex(colorIndex) {}
};

class VoxelModel: public GameObject {
public:
	//voxels
	std::vector<Voxel> Voxels;   //all of the voxels that make up the model
	void updateVoxels();

	//color palette
	std::vector<unsigned int> palette = std::vector<unsigned int>(256, 0);

	//rendering
	VoxelRenderer* renderer;
	void draw();

	VoxelModel(VoxelRenderer* renderer = nullptr);
};
