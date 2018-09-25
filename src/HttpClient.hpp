
#ifndef HTTPCLIENT_HPP
#define HTTPCLIENT_HPP

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <winsock2.h>

#include <string>

#include "HttpRequest.hpp"
#include "HttpResponse.hpp"

class HttpClient 
{
public:
    HttpClient(SOCKET clientConnection);
    ~HttpClient();

    int Send(HttpResponse response);
    HttpRequest Recieve();
    void Close();

    inline const SOCKET GetSocketConnection() { return _clientConnection; }

private:
    SOCKET _clientConnection;
};

#endif // HTTPCLIENT_HPP