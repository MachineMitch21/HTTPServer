
#include "FileCache.hpp"
#include "FileIO.hpp"

namespace FileUtils
{
    std::map<std::string, LoadedFile> FileCache::_cacheMap;
    std::mutex FileCache::_cacheMutex;

    FileCache::FileCache()
    {
        
    }

    FileCache::~FileCache()
    {
        
    }

    LoadedFile FileCache::Get(const std::string& fileName, bool isImage)
    {
        LoadedFile loadedFile = { };

        auto it = _cacheMap.find(fileName);
        if (it != _cacheMap.end())
        {
            loadedFile = it->second;
        }
        else 
        {
            loadedFile = ReadFile(fileName, isImage);
            if (loadedFile.WasLoaded == true)
            {
                printf("Loaded %s from disk...\n\n", fileName.c_str());
                {
                    std::lock_guard<std::mutex> lock(_cacheMutex);
                    _cacheMap.emplace(fileName, loadedFile);
                }
            }
        }

        return loadedFile;
    }

    void FileCache::Clear()
    {
        _cacheMap.clear();
    }
}

