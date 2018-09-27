
#include "FileIO.hpp"

#include <stdio.h>
#include <stdlib.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace FileUtils 
{
	void ReadImage(const std::string& fileName, LoadedFile& loadedFile)
	{
		int width, height, bytesPerPixel;
		unsigned char* buffer = stbi_load(fileName.c_str(), &width, &height, &bytesPerPixel, 0);
		size_t bufferSize = width * height * bytesPerPixel;
		loadedFile.Data = std::string(buffer, buffer + bufferSize);
		loadedFile.Length = bufferSize;
		printf("Read %d bytes from image file: %s...\n\n", bufferSize, fileName.c_str());
		stbi_image_free(buffer);
	}

	LoadedFile ReadFile(const std::string& fileName, bool isImage)
	{
		LoadedFile loadedFile = { };
		loadedFile.Name = fileName;
		loadedFile.Data = "";

		FILE* fp;
		fp = fopen(fileName.c_str(), "rb");

		if (fp != nullptr)
		{
			fseek(fp, 0, SEEK_END);
			loadedFile.Length = ftell(fp);
			rewind(fp);

			if (isImage)
			{
				unsigned char* buffer = (unsigned char*) malloc(sizeof(unsigned char) * loadedFile.Length);
				fread(buffer, loadedFile.Length, 1, fp);
				loadedFile.Data = std::string(buffer, buffer + loadedFile.Length);
				free(buffer);
			}
			else 
			{
				char* buffer = (char*) malloc(sizeof(char) * loadedFile.Length);
				fread(buffer, loadedFile.Length, 1, fp);
				loadedFile.Data = std::string(buffer);
				free(buffer);
			}
		}
		else 
		{
			printf("Could not open file: %s\n\n", fileName.c_str());
			loadedFile.WasLoaded = false;
			return loadedFile;
		}

		fclose(fp);
		loadedFile.WasLoaded = true;
		return loadedFile;
	}
}