
#ifndef SERVERSOCKET_HPP
#define SERVERSOCKET_HPP

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif 

#include <Windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#include <string>

class ServerSocket 
{
public:
    ServerSocket(std::string port);
    ~ServerSocket();

    void Initialize();
    SOCKET Accept();

    void Close();

private:
    SOCKET              _listenSocket;
    struct addrinfo*    _result;
    struct addrinfo     _hints;
    std::string         _port;
};

#endif // SERVERSOCKET_HPP