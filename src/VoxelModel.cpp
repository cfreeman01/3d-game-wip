#include "VoxelModel.h"
#include "resource_manager.h"

VoxelModel::VoxelModel(VoxelRenderer* renderer): renderer(renderer) {
}

void VoxelModel::draw() {
	for (int i = 0; i < Voxels.size(); i++) {
		//draw voxel
		renderer->drawVoxel(Voxels[i], *this);
	}
}

