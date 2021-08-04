#include "VoxelModel.h"
#include "resource_manager.h"

VoxelModel::VoxelModel(VoxelRenderer* renderer): GameObject(), renderer(renderer) { }

//draw each voxel in the model
void VoxelModel::draw() {
	updateVoxels();
	for (int i = 0; i < Voxels.size(); i++) {
		renderer->drawVoxel(Voxels[i], *this);
	}
}

void VoxelModel::updateVoxels() {
	for (int i = 0; i < Voxels.size(); i++) {
		Voxels[i].rotate = rotate;
		Voxels[i].scale = scale;
		Voxels[i].pos = pos + glm::vec3(Voxels[i].x * scale, Voxels[i].y * scale, Voxels[i].z * scale);
	}
}

