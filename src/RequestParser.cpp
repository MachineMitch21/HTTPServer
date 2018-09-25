
#include "RequestParser.hpp"

RequestParser::HTTPRequestMap RequestParser::ParseRequest(HttpRequest httpRequest)
{
    RequestParser::HTTPRequestMap requestMap;

    size_t endSubStrLoc = 0;
    endSubStrLoc = httpRequest.Data.find_first_of(' ');

    std::string httpMethod = httpRequest.Data.substr(0, endSubStrLoc);
    httpRequest.Data.erase(0, endSubStrLoc + 1);

    endSubStrLoc = httpRequest.Data.find_first_of(' ');
    std::string httpMethodValue = httpRequest.Data.substr(0, endSubStrLoc);

    httpRequest.Data.erase(0, endSubStrLoc + 1);

    requestMap.emplace(httpMethod, httpMethodValue);

    endSubStrLoc = httpRequest.Data.find_first_of('\n');
    httpRequest.Data.erase(0, endSubStrLoc + 1);

    while (true)
    {
        endSubStrLoc = httpRequest.Data.find_first_of(':');

        if (endSubStrLoc == std::string::npos)
        {
            break;
        }
        else 
        {
            std::string httpKey = httpRequest.Data.substr(0, endSubStrLoc);
            httpRequest.Data.erase(0, endSubStrLoc + 2);

            endSubStrLoc = httpRequest.Data.find_first_of('\n');
            std::string httpValue = httpRequest.Data.substr(0, endSubStrLoc);

            httpRequest.Data.erase(0, endSubStrLoc + 1);
            requestMap.emplace(httpKey, httpValue);
        }
    }

    return requestMap;
}