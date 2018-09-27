
#ifndef FILECACHE_HPP
#define FILECACHE_HPP

#include <string>
#include <map>

class FileCache 
{
public:
    FileCache();
    ~FileCache();

private:
    std::map<std::string, std::string> _cacheMap;
};

#endif // FILECACHE_HPP