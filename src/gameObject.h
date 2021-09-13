#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/geometric.hpp>

/*base class for all in-game objects (player, enemies, environment)
this class holds the data needed for the game logic (collision detection, etc.)
the render data (relative positions and colors of voxels) is constant and stored in the VoxelModel class*/
class GameObject
{
public:
	glm::vec3 pos;					//world position
	glm::vec3 size;					//number of voxels in each direction
	glm::vec3 rotate;				//rotation around x, y, and z axis
	glm::vec3 tintColor;			//color to render on top of model (red after character takes damage)
	float scale = 1.0f;				//scale of each voxel (so scaled size can be obtained by scale*size)
	float distanceOffGround = 0.0f; //how far off the ground the object is rendered

	GameObject(glm::vec3 pos = glm::vec3(0, 0, 0), glm::vec3 size = glm::vec3(1, 1, 1), glm::vec3 rotate = glm::vec3(0, 0, 0),
			   float scale = 1.0f) : pos(pos), size(size), scale(scale), tintColor(glm::vec3(1.0f, 1.0f, 1.0f)) {}
};