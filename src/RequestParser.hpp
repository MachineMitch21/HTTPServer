
#ifndef REQUESTPARSER_HPP
#define REQUESTPARSER_HPP

#include <map>
#include <string>

#include "HttpRequest.hpp"

namespace RequestParser 
{
    typedef std::map<std::string, std::string> HTTPRequestMap;
    
    HTTPRequestMap ParseRequest(HttpRequest httpRequest);
};

#endif // REQUESTPARSER_HPP