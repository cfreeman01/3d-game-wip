#include "VoxelLoader.h"
#include <fstream>
#include <cmath>
#include <iomanip>
#include <cstring>
#include <iostream>

std::map<std::string, VoxelModel> VoxelLoader::models;
bool displayFileContents = true;

VoxelModel& VoxelLoader::loadModel(const char* file, std::string name) {
	loadModelFromFile(file, name);
	return models[name + "_0"];
}

VoxelModel& VoxelLoader::getModel(std::string name) {
	return models[name];
}

//parse a VOX file and store the data in a VoxelModel object
//VOX file format specification: https://github.com/ephtracy/voxel-model/blob/master/MagicaVoxel-file-format-vox.txt
void VoxelLoader::loadModelFromFile(const char* file, std::string name) {
	char bytes[4];
	std::ifstream input(file, std::ios::binary);
	if (input.fail()) {
		std::cout << "Could not open file " << file;
		exit(1);
	}

	input.read(bytes,4);   //check if starting id is correct
	if (!(strncmp(bytes, "VOX ", 4) == 0)) {
		std::cout << "Incorrect file format";
		exit(1);
	}
	input.read(bytes, 4);  //read version number

	//parse MAIN chunk
	input.read(bytes, 4);  //id
	input.read(bytes, 4);  //num bytes of chunk content in MAIN (should be 0)
	input.read(bytes, 4);  //num bytes of chunk content for rest of file

	unsigned int numBytes = byteArrayToInt(bytes);
	char* buffer = new char[numBytes];

	input.read(buffer, numBytes);
	if (!input) {
		std::cout << "Error reading file contents.";
		exit(1);
	}

	int i = 0;        //index into buffer
	int modelNum = 0; //number of models read from file

	while (i < numBytes) {  //main loop to read file
		getNextFourBytes(bytes, buffer, i);

		//READ MODEL FROM FILE---------
		if (strncmp(bytes, "SIZE", 4) == 0) {
			VoxelModel& newModel = models[name + "_" + (char)('0' + modelNum)] = VoxelModel();
			modelNum++;
			getNextFourBytes(bytes, buffer, i);  //read num bytes of chunk content (should be 12)
			getNextFourBytes(bytes, buffer, i);  //read num bytes of children chunks (should be 0)

			getNextFourBytes(bytes, buffer, i); /*read x,y,z size. not too useful since it includes */
			getNextFourBytes(bytes, buffer, i); /*the entire workspace (including empty voxels)     */
			getNextFourBytes(bytes, buffer, i); /*so the actual size is calculated later on         */
			
			getNextFourBytes(bytes, buffer, i); //should be 'XYZI'
			if (strncmp(bytes, "XYZI", 4) != 0) {
				std::cout << "Error reading model.";
				exit(1);
			}
			getNextFourBytes(bytes, buffer, i);    //read num bytes of chunk content
			getNextFourBytes(bytes, buffer, i);    //read num bytes of children chunks

			int numVoxels = byteArrayToInt(getNextFourBytes(bytes, buffer, i));
			int minX = INT_MAX, minY = INT_MAX, minZ = INT_MAX;
			int maxX = INT_MIN, maxY = INT_MIN, maxZ = INT_MIN;

			for (int j = 0; j < numVoxels; j++) {  //get the voxels
				getNextFourBytes(bytes, buffer, i);
				newModel.Voxels.push_back(Voxel(
					(int)(unsigned char)bytes[0], //x
					(int)(unsigned char)bytes[2], //y
					(int)(unsigned char)bytes[1], //z
					(int)(unsigned char)bytes[3]  //colorIndex
				));

				//check min and max x values (to calculate size)
				if (newModel.Voxels.back().x < minX) minX = newModel.Voxels.back().x;
				if (newModel.Voxels.back().x > maxX) maxX = newModel.Voxels.back().x;
				if (newModel.Voxels.back().y < minY) minY = newModel.Voxels.back().y;
				if (newModel.Voxels.back().y > maxY) maxY = newModel.Voxels.back().y;
				if (newModel.Voxels.back().z < minZ) minZ = newModel.Voxels.back().z;
				if (newModel.Voxels.back().z > maxZ) maxZ = newModel.Voxels.back().z;
			}
			//set size
			newModel.size.x = (maxX - minX) + 1;
			newModel.size.y = (maxY - minY) + 1;
			newModel.size.z = (maxZ - minZ) + 1;
			//adjust position so that (minX,minY,minZ) is at (0,0,0)
			for (int i = 0; i < newModel.Voxels.size(); i++) {
				Voxel& vox = newModel.Voxels[i];
				vox.x -= minX;
				vox.y -= minY;
				vox.z -= minZ;
			}
		}
		//----------------------------

		//Read color palette----------
		if (strncmp(bytes, "RGBA", 4) == 0) {
			getNextFourBytes(bytes, buffer, i);    //read num bytes of chunk content
			getNextFourBytes(bytes, buffer, i);    //read num bytes of children chunks

			std::vector<unsigned int> newPalette(256, 0x00000000);  //read the new palette
			for (int  j = 1; j < 256; j++) {
				newPalette[j] = byteArrayToInt(getNextFourBytes(bytes, buffer, i));
			}

			for (int j = 0; j < modelNum; j++) {  //assign the palette to all models from this file
				models[name + "_" + (char)('0' + j)].palette = newPalette;
			}
		}
		//----------------------------

		if (strncmp(bytes, "MATL", 4) == 0) break;  //materials are ignored
	}

	delete[] buffer;
}

//convert a 4-byte array to an unsigned int
unsigned int VoxelLoader::byteArrayToInt(char bytes[4]) {
	unsigned int ret = 0;
	unsigned int b0 = (unsigned int)((unsigned char)bytes[0]); //least significant byte
	unsigned int b1 = (unsigned int)((unsigned char)bytes[1]) << 8;
	unsigned int b2 = (unsigned int)((unsigned char)bytes[2]) << 16;
	unsigned int b3 = (unsigned int)((unsigned char)bytes[3]) << 24;
	ret = ret | b0 | b1 | b2 | b3;
	return ret;
}
//get next four bytes from buffer and place in bytes array
char* VoxelLoader::getNextFourBytes(char bytes[4], char* buffer, int& i) {
	for (int j = 0; j < 4; j++) bytes[j] = buffer[i++];
	return bytes;
}