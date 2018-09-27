
#ifndef FILEUTILS_HPP
#define FILEUTILS_HPP

#include <string>

#include "LoadedFile.hpp"

namespace FileUtils 
{
    LoadedFile ReadFile(const std::string& fileName, bool isImage = false);
};

#endif // FILEUTILS_HPP