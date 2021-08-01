#include "level.h"

Level::Level(VoxelRenderer& renderer, Game& game) : renderer(renderer), game(game) {
	islands.push_back(VoxelLoader::loadModel("models/first_island.vox", "first_island"));
	islands[0].pos = glm::vec3(-(float)islands[0].sizeX / 2, -(float)islands[0].sizeY / 2, -(float)islands[0].sizeZ / 2);
	renderer.shader.SetVector3f("lightPos", lightPos);
	renderer.shader.SetVector3f("lightColor", lightColor);
	islands[0].renderer = &renderer;
}

void Level::drawIslands() {
	for (int i = 0; i < islands.size(); i++) {
		islands[i].draw();
	}
}

