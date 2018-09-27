
#ifndef LOADEDFILE_HPP
#define LOADEDFILE_HPP

#include <string>

namespace FileUtils 
{
    struct LoadedFile
    {
        std::string Name;
        std::string Data;
        size_t      Length;
        bool        WasLoaded = false;
    };
}

#endif // LOADEDFILE_HPP