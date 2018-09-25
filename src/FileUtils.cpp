
#include "FileUtils.hpp"

#include <stdio.h>
#include <stdlib.h>

namespace FileUtils 
{
	LoadedFile ReadFile(const std::string& fileName)
	{
		LoadedFile loadedFile = { };

		FILE* fp;
		fp = fopen(fileName.c_str(), "rb");

		if (fp != nullptr)
		{
			fseek(fp, 0, SEEK_END);
			loadedFile.Length = ftell(fp);
			fseek(fp, 0, SEEK_SET);

			printf("Reading %d bytes from %s\n\n", loadedFile.Length, fileName.c_str());

			char* buffer = (char*) malloc(sizeof(char) * loadedFile.Length);
			memset(buffer, 0, sizeof(char) * loadedFile.Length + 1);
			fread(buffer, loadedFile.Length, 1, fp);
			loadedFile.Data = std::string(buffer);
			free(buffer);
		}
		else 
		{
			printf("Could not open file: %s\n", fileName.c_str());
		}

		return loadedFile;
	}
}