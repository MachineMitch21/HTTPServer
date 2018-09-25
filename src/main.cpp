
#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include "ServerSocket.hpp"
#include "ConnectionManager.hpp"

#define DEFAULT_BUFLEN  512
#define DEFAULT_PORT    "80"

int main(int argc, char** argv)
{
    SOCKET ClientSocket = INVALID_SOCKET;

    ServerSocket server(std::string(DEFAULT_PORT));
    server.Initialize();

    ConnectionManager connectionManager;

    while (true)
    {
        ClientSocket = server.Accept();

        if (ClientSocket != INVALID_SOCKET)
        {
            connectionManager.PushConnection(ClientSocket);
        }
        else 
        {
            ExitProcess(1);
        }

        
    }

    connectionManager.Stop();

    return 0;
}