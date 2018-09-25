
#include "FileUtils.hpp"

#include <stdio.h>
#include <stdlib.h>

namespace FileUtils 
{
	LoadedFile ReadFile(const std::string& fileName)
	{
		LoadedFile loadedFile = { };

		FILE* fp;
		fp = fopen(fileName.c_str(), "r");

		if (fp != nullptr)
		{
			fseek(fp, 0, SEEK_END);
			loadedFile.Length = ftell(fp);
			fseek(fp, 0, SEEK_SET);

			char buffer[512];
			memset(buffer, 0, 512);
			while (fread(buffer, sizeof(char), 512, fp) > 0)
			{
				printf("Reading %s from disk...\n", fileName.c_str());
				loadedFile.Data += std::string(buffer);
				memset(buffer, 0, 512);
			}
		}
		else 
		{
			printf("Could not open file: %s\n", fileName.c_str());
		}

		return loadedFile;
	}
}