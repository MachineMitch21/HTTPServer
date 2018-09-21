
#ifndef CONNECTIONMANAGER_HPP
#define CONNECTIONMANAGER_HPP

#include <winsock2.h>
#include <ws2tcpip.h>

#include <queue>
#include <thread>
#include <mutex>
#include <atomic>

class ConnectionManager 
{
public:
    ConnectionManager();
    ~ConnectionManager();

    void PushConnection(SOCKET client);
    void Stop();

private:
    std::condition_variable     _conditionVar;
    std::mutex                  _connectionMutex;
    std::queue<std::thread>     _connections;
    std::thread                 _connectionThread;
    std::atomic<bool>           _running = true;
};

#endif // CONNECTIONMANAGER_HPP