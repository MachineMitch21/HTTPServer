
#ifndef CONNECTIONMANAGER_HPP
#define CONNECTIONMANAGER_HPP

#include <winsock2.h>
#include <ws2tcpip.h>

#include <queue>
#include <map>
#include <thread>
#include <mutex>
#include <atomic>

class ConnectionManager 
{
public:
    ConnectionManager();
    ~ConnectionManager();

    void PushConnection(SOCKET client);
    void PollConnections();
    void Stop();

private:
    std::mutex                      _connectionMutex;
    std::queue<std::thread>         _connections;
    std::map<std::thread::id, bool> _connectionSendFlags;
    std::thread                     _connectionThread;
    std::atomic<bool>               _running = true;
};

#endif // CONNECTIONMANAGER_HPP