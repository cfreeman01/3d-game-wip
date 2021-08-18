#pragma once
#include <map>
#include <string>
#include <vector>
#include <glad/glad.h>

//FORWARD DECLARATIONS
class VoxelModel;
class VoxelRenderer;

/*loads data from .VOX files and stores it in VoxelModel objects. the VoxelModel objects are stored in a static map
and are retrieved via reference (so there is only one instance of each model). this is sufficient since the VoxelModel
data does not change, so all changing data (like world position) can be stored in other data types that hold a reference
to the appropriate VoxelModel*/
class VoxelLoader {
private:
	//storage of voxel models
	static std::map<std::string, VoxelModel> models;

public:
	//load a voxel model from a .vox file
	static VoxelModel& loadModel(const char* file, std::string name);
	static void loadModelFromFile(const char* file, std::string name);

	//get a voxel model from storage
	static VoxelModel& getModel(std::string name);

	static unsigned int VoxelLoader::byteArrayToInt(char bytes[4]);
	static char* getNextFourBytes(char bytes[4], char* buffer, int& i);
};