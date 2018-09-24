
#include "ConnectionManager.hpp"

#include <sstream>

#include "RequestParser.hpp"
#include "FileUtils.hpp"

// TODO: Do the same with this as the one below
struct HTTPResponse
{
    std::string Header;
    std::string Body;
};

// TODO: Actually put these into a file and implement them
class HTTPClient 
{
public:
    HTTPClient(SOCKET WinSockClient);

    int Send(HTTPResponse response);
    int Recieve();
    void Close();

private:

};

void ConnectionThread(std::queue<std::thread>& connections, std::condition_variable& condVar, std::mutex& mu, std::atomic<bool>& running)
{
    while (running)
    {
        if (!connections.empty())
        {
            std::thread t;
            std::unique_lock<std::mutex> lock(mu);
            condVar.wait(lock);
            t = std::move(connections.front());
            connections.pop();
            lock.unlock();

            t.join();
        }
    }

    while (!connections.empty())
    {
        std::thread t;
        std::unique_lock<std::mutex> lock(mu);
        condVar.wait(lock);
        t = std::move(connections.front());
        connections.pop();
        lock.unlock();

        t.join();    
    }
}

void RequestHandler(SOCKET client)
{
    char recvbuf[512];

    int iResult;
    int iSendResult;

    iResult = recv(client, recvbuf, 512, 0);
    if (iResult > 0) 
    {
        RequestParser::HTTPRequest http_request = RequestParser::ParseRequest(std::string(recvbuf));

        std::string responseHeader = std::string("HTTP/1.1 200 OK\n");

        std::string httpDoc =   std::string("<!DOCTYPE html>\n")            + 
                                std::string("<html>\n")                     +
                                std::string("<head> \n")                    +
                                std::string("   <meta charset='utf-8'>\n")  +
                                std::string("</head>\n")                    +
                                std::string("<body>\n")                     +
                                std::string("   <h1>Hello, World!</h1>\n")  +
                                std::string("</body>\n")                    +
                                std::string("</html>\n");

        if (http_request.find("GET") != http_request.end())
        {
            if (http_request["GET"] != "/" && http_request["GET"] != "/index.html")
            {
                std::string fileName = http_request["GET"];
                size_t slashLoc = fileName.find_first_of('/');
                
                if (slashLoc != std::string::npos)
                {
                    fileName.erase(0, slashLoc + 1);
                }

                httpDoc.clear();
                httpDoc = FileUtils::ReadFile(fileName);

                if (fileName.find(".css") != std::string::npos)
                {
                    responseHeader += "Content-Type: text/css\n";
                }
                else if (fileName.find(".js") != std::string::npos)
                {
                    responseHeader += "Content-Type: text/javascript\n";
                }
            }
            else 
            {
                httpDoc.clear();
                httpDoc = FileUtils::ReadFile("index.html");

                responseHeader += "Content-Type: text/html\n";
            }
        }

        printf("HTTPResponse body length: %d\n", httpDoc.length());

        std::ostringstream s;
        s << "Content-Length: " << httpDoc.length() << "\n\n";

        responseHeader += s.str();

        std::string sendStr = responseHeader + httpDoc;

        iSendResult = send( client, sendStr.c_str(), sendStr.length(), 0 );
        if (iSendResult == SOCKET_ERROR) {
            printf("send failed with error: %d\n", WSAGetLastError());
            closesocket(client);
            WSACleanup();
        }
    }
    else if (iResult == 0)
        closesocket(client);
    else  
    {
        printf("recv failed with error: %d\n", WSAGetLastError());
        closesocket(client);
        WSACleanup();
    }

    closesocket(client);  
}

ConnectionManager::ConnectionManager()
{
    _connectionThread = std::thread(&ConnectionThread, std::ref(_connections), std::ref(_conditionVar), std::ref(_connectionMutex), std::ref(_running));
}

ConnectionManager::~ConnectionManager()
{
    Stop();
    _connectionThread.join();
}

void ConnectionManager::PushConnection(SOCKET client)
{
    std::unique_lock<std::mutex> lock(_connectionMutex);
    _connections.push(std::move(std::thread(RequestHandler, client)));
    lock.unlock();
    _conditionVar.notify_one();
}

void ConnectionManager::Stop()
{
    _running = false;
}