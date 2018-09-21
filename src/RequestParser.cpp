
#include "RequestParser.hpp"

RequestParser::HTTPRequest RequestParser::ParseRequest(std::string request)
{
    std::map<std::string, std::string> requestMap;

    size_t endSubStrLoc = 0;
    endSubStrLoc = request.find_first_of(' ');

    std::string httpMethod = request.substr(0, endSubStrLoc);
    request.erase(0, endSubStrLoc + 1);

    endSubStrLoc = request.find_first_of(' ');
    std::string httpMethodValue = request.substr(0, endSubStrLoc);

    request.erase(0, endSubStrLoc + 1);

    requestMap.emplace(httpMethod, httpMethodValue);

    endSubStrLoc = request.find_first_of('\n');
    request.erase(0, endSubStrLoc + 1);

    while (true)
    {
        endSubStrLoc = request.find_first_of(':');

        if (endSubStrLoc == std::string::npos)
        {
            break;
        }
        else 
        {
            std::string httpKey = request.substr(0, endSubStrLoc);
            request.erase(0, endSubStrLoc + 2);

            endSubStrLoc = request.find_first_of('\n');
            std::string httpValue = request.substr(0, endSubStrLoc);

            request.erase(0, endSubStrLoc + 1);
            requestMap.emplace(httpKey, httpValue);
        }
    }

    return requestMap;
}