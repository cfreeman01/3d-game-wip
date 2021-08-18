#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "gameObject.h"

//FORWARD DECLARATIONS
class VoxelRenderer;

/*holds voxel data that is passed to the shader*/
struct VoxelRenderData {
	float x, y, z;   //local position
	float R, G, B;   //color
	int colorIndex;
};

/*holds the data for a voxel model obtained from a .VOX file, using the VoxelLoader class
this data does not change, so multiple game objects can hold references to the same VoxelModel
(for example if there are multiples enemeies of the same type, etc.)*/
class VoxelModel{
	friend class VoxelRenderer;
	friend class VoxelLoader;
private:
	VoxelRenderData* vRenderData; //render data stored as dynamically allocated array
	int numVoxels;
	glm::vec3 size;
public:
	int getNumVoxels() { return numVoxels; }
	glm::vec3 getSize() { return size; }
	glm::vec3 getVoxPos(int i);
};
