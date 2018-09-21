
#ifndef REQUESTPARSER_HPP
#define REQUESTPARSER_HPP

#include <map>
#include <string>

namespace RequestParser 
{
    typedef std::map<std::string, std::string> HTTPRequest;
    
    HTTPRequest ParseRequest(std::string request);
};

#endif // REQUESTPARSER_HPP