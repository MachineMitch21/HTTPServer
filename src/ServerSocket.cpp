
#include "ServerSocket.hpp"

#include <stdio.h>

ServerSocket::ServerSocket(std::string port)
    :   _listenSocket(INVALID_SOCKET),
        _result(NULL),
        _hints({}),
        _port(port)
{

}

ServerSocket::~ServerSocket()
{
    if (_listenSocket != INVALID_SOCKET)
    {
        Close();
    }

    WSACleanup();
}

void ServerSocket::Initialize()
{
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

    if (iResult != 0)
    {
        printf("WSAStartup failed with error: %d\n", iResult);
        ExitProcess(iResult);
    }

    _hints.ai_family     = AF_INET;
    _hints.ai_socktype   = SOCK_STREAM;
    _hints.ai_protocol   = IPPROTO_TCP;
    _hints.ai_flags      = AI_PASSIVE;

    // Resolve the server address and port
    iResult = getaddrinfo(NULL, _port.c_str(), &_hints, &_result);
    if (iResult != 0) 
    {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        ExitProcess(iResult);
    }

    // Create a SOCKET for connecting to server
    _listenSocket = socket(_result->ai_family, _result->ai_socktype, _result->ai_protocol);
    if (_listenSocket == INVALID_SOCKET) 
    {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(_result);
        WSACleanup();
        ExitProcess(WSAGetLastError());
    }

    // Setup the TCP listening socket
    iResult = bind( _listenSocket, _result->ai_addr, (int)_result->ai_addrlen);
    if (iResult == SOCKET_ERROR) 
    {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(_result);
        closesocket(_listenSocket);
        WSACleanup();
        ExitProcess(WSAGetLastError());
    }

    freeaddrinfo(_result);

    iResult = listen(_listenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR)
    {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(_listenSocket);
        WSACleanup();
        ExitProcess(iResult);
    }
}

SOCKET ServerSocket::Accept()
{
    SOCKET client = accept(_listenSocket, NULL, NULL);
    return client;
}

void ServerSocket::Close()
{
    closesocket(_listenSocket);
}