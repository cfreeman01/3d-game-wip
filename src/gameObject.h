#pragma once
#include "game.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/geometric.hpp>

//base class that holds basic data for voxels and voxel models
class GameObject {
public:
	glm::vec3 pos;  //world position
	glm::vec3 size; //number of voxels in each direction
	glm::vec3 rotate;     //rotation around x, y, and z axis
	float scale = 1.0f;  //scale of each voxel (so scaled size can be obtained by scale*size)

	GameObject(glm::vec3 pos = glm::vec3(0,0,0), glm::vec3 size = glm::vec3(1,1,1), glm::vec3 rotate = glm::vec3(0,0,0),
		float scale = 1.0f) : pos(pos), size(size), scale(scale) {}
};