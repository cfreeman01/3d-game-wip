#include "VoxelModel.h"
#include "resource_manager.h"
#include "GameObject.h"
#include "VoxelRenderer.h"

glm::vec3 VoxelModel::getVoxPos(int i) { //return the local position of the voxel at vRenderData[i]
	return glm::vec3(vRenderData[i].x, vRenderData[i].y, vRenderData[i].z);
}