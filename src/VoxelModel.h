#pragma once
#include "VoxelRenderer.h"
#include <glm/glm.hpp>
#include <vector>

struct Voxel {
	int x, y, z;              //x,y,z position relative to model's position
	unsigned int colorIndex;  //index into model's color palette
	Voxel(unsigned int x, unsigned int y, unsigned int z, unsigned int colorIndex) : x(x), y(y), z(z), colorIndex(colorIndex) {}
};

class VoxelModel {
public:
	glm::vec3 pos = glm::vec3(0.0f,0.0f,0.0f);   //position
	int sizeX = 0, sizeY = 0, sizeZ = 0;         //number of voxels in each direction
	float rotateX = 0.0f, rotateY = 0.0f, rotateZ = 0.0f;
	float scale = 1.0f;

	std::vector<Voxel> Voxels;
	std::vector<unsigned int> palette = std::vector<unsigned int>(256, 0);

	//rendering
	VoxelRenderer* renderer;
	void draw();

	VoxelModel(VoxelRenderer* renderer = nullptr);
};
