#include "VoxelModel.h"
#include "resource_manager.h"

VoxelModel::VoxelModel(VoxelRenderer* renderer): renderer(renderer) { }

//draw each voxel in the model
void VoxelModel::draw() {
	for (int i = 0; i < Voxels.size(); i++) {
		renderer->drawVoxel(Voxels[i], *this);
	}
}

