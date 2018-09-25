
#ifndef HTTPRESPONSE_HPP
#define HTTPRESPONSE_HPP

#include <string>

struct HttpResponse 
{
    std::string Header;
    std::string Body;
    size_t      BodyLength;
};

#endif // HTTPRESPONSE_HPP