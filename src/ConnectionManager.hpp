
#ifndef SERVER_HPP
#define SERVER_HPP

#include <winsock2.h>
#include <ws2tcpip.h>

#include <queue>

#include <thread>

void RequestHandler(SOCKET client)
{

}

class ConnectionManager 
{
public:
    ConnectionManager();
    ~ConnectionManager();

    void PushConnection(SOCKET client);

private:
    std::queue<std::thread> _connections;
};

#endif // SERVER_HPP