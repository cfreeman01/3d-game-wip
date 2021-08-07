#pragma once
#include <map>
#include <string>
#include <vector>
#include <glad/glad.h>

//FORWARD DECLARATIONS
class VoxelModel;
class VoxelRenderer;

//loads voxel model from .vox files and stores them in appropriate data structures
class VoxelLoader {
public:
	//storage of voxel models
	static std::map<std::string, VoxelModel> models;

	//load a voxel model from a .vox file
	static VoxelModel& loadModel(const char* file, std::string name);

	//get a voxel model from storage
	static VoxelModel& getModel(std::string name);

	static void loadModelFromFile(const char* file, std::string name);

	static unsigned int VoxelLoader::byteArrayToInt(char bytes[4]);
	static char* getNextFourBytes(char bytes[4], char* buffer, int& i);
};