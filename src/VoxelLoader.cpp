#include <fstream>
#include <cmath>
#include <iomanip>
#include <cstring>
#include <iostream>
#include "VoxelLoader.h"
#include "VoxelModel.h"
#include "VoxelRenderer.h"

std::map<std::string, VoxelModel> VoxelLoader::models;
bool displayFileContents = true;

VoxelModel &VoxelLoader::loadModel(const char *file, std::string name)
{
	loadModelFromFile(file, name);
	return models[name];
}

VoxelModel &VoxelLoader::getModel(std::string name)
{
	return models[name];
}

//parse a VOX file and store the data in a VoxelModel object
//VOX file format specification: https://github.com/ephtracy/voxel-model/blob/master/MagicaVoxel-file-format-vox.txt
void VoxelLoader::loadModelFromFile(const char *file, std::string name)
{
	char bytes[4];
	std::ifstream input(file, std::ios::binary);
	if (input.fail())
	{
		std::cout << "Could not open file " << file;
		exit(1);
	}

	input.read(bytes, 4); //check if starting id is correct
	if (!(strncmp(bytes, "VOX ", 4) == 0))
	{
		std::cout << "Incorrect file format";
		exit(1);
	}
	input.read(bytes, 4); //read version number

	//parse MAIN chunk
	input.read(bytes, 4); //id
	input.read(bytes, 4); //num bytes of chunk content in MAIN (should be 0)
	input.read(bytes, 4); //num bytes of chunk content for rest of file

	unsigned int numBytes = byteArrayToInt(bytes);
	char *buffer = new char[numBytes];

	input.read(buffer, numBytes);
	if (!input)
	{
		std::cout << "Error reading file contents.";
		exit(1);
	}

	int i = 0;		  //index into buffer
	int modelNum = 0; //number of models read from file

	while (i < numBytes)
	{ //main loop to read file
		getNextFourBytes(bytes, buffer, i);

		//READ MODEL FROM FILE---------
		if (strncmp(bytes, "SIZE", 4) == 0)
		{
			std::string newName;
			if (modelNum > 0)
				newName = name + "_" + (char)('0' + modelNum);
			else
				newName = name;
			VoxelModel &newModel = models[newName] = VoxelModel();
			modelNum++;
			getNextFourBytes(bytes, buffer, i); //read num bytes of chunk content (should be 12)
			getNextFourBytes(bytes, buffer, i); //read num bytes of children chunks (should be 0)

			getNextFourBytes(bytes, buffer, i); /*read x,y,z size. not too useful since it includes */
			getNextFourBytes(bytes, buffer, i); /*the entire workspace (including empty voxels)     */
			getNextFourBytes(bytes, buffer, i); /*so the actual size is calculated later on         */

			getNextFourBytes(bytes, buffer, i); //should be 'XYZI'
			if (strncmp(bytes, "XYZI", 4) != 0)
			{
				std::cout << "Error reading model.";
				exit(1);
			}
			getNextFourBytes(bytes, buffer, i); //read num bytes of chunk content
			getNextFourBytes(bytes, buffer, i); //read num bytes of children chunks

			int numVoxels = byteArrayToInt(getNextFourBytes(bytes, buffer, i));
			newModel.vRenderData = new VoxelRenderData[numVoxels]; //initialize model's VoxelRenderData array
			newModel.numVoxels = numVoxels;
			int minX = INT_MAX, minY = INT_MAX, minZ = INT_MAX;
			int maxX = INT_MIN, maxY = INT_MIN, maxZ = INT_MIN;

			for (int j = 0; j < numVoxels; j++)
			{ //get the voxels
				getNextFourBytes(bytes, buffer, i);
				newModel.vRenderData[j].x = (int)(unsigned char)bytes[0];		   //x
				newModel.vRenderData[j].y = (int)(unsigned char)bytes[2];		   //y
				newModel.vRenderData[j].z = (int)(unsigned char)bytes[1];		   //z
				newModel.vRenderData[j].colorIndex = (int)(unsigned char)bytes[3]; //colorIndex

				//check min and max x values (to calculate size)
				if (newModel.vRenderData[j].x < minX)
					minX = newModel.vRenderData[j].x;
				if (newModel.vRenderData[j].x > maxX)
					maxX = newModel.vRenderData[j].x;
				if (newModel.vRenderData[j].y < minY)
					minY = newModel.vRenderData[j].y;
				if (newModel.vRenderData[j].y > maxY)
					maxY = newModel.vRenderData[j].y;
				if (newModel.vRenderData[j].z < minZ)
					minZ = newModel.vRenderData[j].z;
				if (newModel.vRenderData[j].z > maxZ)
					maxZ = newModel.vRenderData[j].z;
			}
			//set size
			newModel.size.x = (maxX - minX) + 1;
			newModel.size.y = (maxY - minY) + 1;
			newModel.size.z = (maxZ - minZ) + 1;
			//adjust position so that (minX,minY,minZ) is at (0,0,0)
			for (int i = 0; i < numVoxels; i++)
			{
				VoxelRenderData &vox = newModel.vRenderData[i];
				vox.x -= minX;
				vox.y -= minY;
				vox.z -= minZ;
			}
		}
		//----------------------------

		//Read color palette----------
		if (strncmp(bytes, "RGBA", 4) == 0)
		{
			getNextFourBytes(bytes, buffer, i); //read num bytes of chunk content
			getNextFourBytes(bytes, buffer, i); //read num bytes of children chunks

			std::vector<unsigned int> newPalette(256, 0x00000000); //read the new palette
			for (int j = 1; j < 256; j++)
			{
				newPalette[j] = byteArrayToInt(getNextFourBytes(bytes, buffer, i));
			}

			for (int j = 0; j < modelNum; j++)
			{ //assign colors to all of the voxels
				std::string modelName;
				if (j > 0)
					modelName = name + "_" + (char)('0' + j);
				else
					modelName = name;
				VoxelModel &currentModel = models[modelName];
				for (int k = 0; k < currentModel.numVoxels; k++)
				{
					//get color from palette
					unsigned int colorInt = newPalette[currentModel.vRenderData[k].colorIndex];
					//calculate RGB components
					unsigned int R = (colorInt & 0x000000ff);
					unsigned int G = (colorInt & 0x0000ff00) >> 8;
					unsigned int B = (colorInt & 0x00ff0000) >> 16;
					//assign the color to the appropriate voxel
					currentModel.vRenderData[k].R = (float)R / 255;
					currentModel.vRenderData[k].G = (float)G / 255;
					currentModel.vRenderData[k].B = (float)B / 255;
				}
			}
		}
		//----------------------------

		if (strncmp(bytes, "MATL", 4) == 0)
			break; //materials are ignored
	}

	delete[] buffer;
}

//convert a 4-byte array to an unsigned int
unsigned int VoxelLoader::byteArrayToInt(char bytes[4])
{
	unsigned int ret = 0;
	unsigned int b0 = (unsigned int)((unsigned char)bytes[0]); //least significant byte
	unsigned int b1 = (unsigned int)((unsigned char)bytes[1]) << 8;
	unsigned int b2 = (unsigned int)((unsigned char)bytes[2]) << 16;
	unsigned int b3 = (unsigned int)((unsigned char)bytes[3]) << 24;
	ret = ret | b0 | b1 | b2 | b3;
	return ret;
}

//get next four bytes from buffer and place in bytes array
char *VoxelLoader::getNextFourBytes(char bytes[4], char *buffer, int &i)
{
	for (int j = 0; j < 4; j++)
		bytes[j] = buffer[i++];
	return bytes;
}