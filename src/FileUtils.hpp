
#ifndef FILEUTILS_HPP
#define FILEUTILS_HPP

#include <string>

namespace FileUtils 
{
    struct LoadedFile
    {
        std::string Data;
        size_t      Length;
    };

    LoadedFile ReadFile(const std::string& fileName);
};

#endif // FILEUTILS_HPP