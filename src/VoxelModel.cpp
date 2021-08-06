#include "VoxelModel.h"
#include "resource_manager.h"
#include "GameObject.h"
#include "VoxelRenderer.h"

VoxelModel::VoxelModel(VoxelRenderer* renderer): GameObject(), renderer(renderer), vRenderData(nullptr){ }

//draw each voxel in the model
void VoxelModel::draw() {
	updateVoxels();
	renderer->drawVoxelModel(*this);
}

void VoxelModel::updateVoxels() {
	for (int i = 0; i < Voxels.size(); i++) {
		Voxels[i].rotate = rotate;
		Voxels[i].scale = scale;
		Voxels[i].pos = pos + glm::vec3(vRenderData[i].x * scale, vRenderData[i].y * scale, vRenderData[i].z * scale);
	}
}

