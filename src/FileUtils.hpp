
#ifndef FILEUTILS_HPP
#define FILEUTILS_HPP

#include <string>

namespace FileUtils 
{
    struct LoadedFile
    {
        std::string Data;
        size_t      Length;
        bool        WasLoaded;
    };

    LoadedFile ReadFile(const std::string& fileName, bool isImage = false);
};

#endif // FILEUTILS_HPP