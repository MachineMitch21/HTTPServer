
#ifndef FILECACHE_HPP
#define FILECACHE_HPP

#include <string>
#include <map>
#include <mutex>

#include "LoadedFile.hpp"

namespace FileUtils
{
    class FileCache 
    {
    public:
        FileCache();
        ~FileCache();

        static LoadedFile Get(const std::string& fileName, bool isImage = false);
        static void Clear();

    private:
        static std::map<std::string, LoadedFile>    _cacheMap;
        static std::mutex                           _cacheMutex;
    };
}


#endif // FILECACHE_HPP