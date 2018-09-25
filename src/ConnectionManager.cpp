
#include "ConnectionManager.hpp"

#include <sstream>
#include <chrono>

#include "RequestParser.hpp"
#include "FileUtils.hpp"
#include "HttpClient.hpp"
#include "HttpRequest.hpp"

void ConnectionThread(std::queue<std::thread>& connections, std::map<std::thread::id, bool>& connectionSendFlags, std::mutex& mu, std::atomic<bool>& running)
{
    while (running)
    {
        if (!connections.empty())
        {
            std::thread t;
            std::unique_lock<std::mutex> lock(mu);
            t = std::move(connections.front());
            connections.pop();
            lock.unlock();

            connectionSendFlags[t.get_id()] = true;
            auto it = connectionSendFlags.find(t.get_id());
            t.join();
            connectionSendFlags.erase(it);
        }
        else 
        {
            std::this_thread::yield();
        }
    }

    while (!connections.empty())
    {
        std::thread t;
        std::unique_lock<std::mutex> lock(mu);
        t = std::move(connections.front());
        connections.pop();
        lock.unlock();

        connectionSendFlags[t.get_id()] = true;
        auto it = connectionSendFlags.find(t.get_id());
        t.join();
        connectionSendFlags.erase(it);   
    }
}

void RequestHandler(HttpClient client, const std::map<std::thread::id, bool>& connectionFlags)
{
    HttpRequest request = client.Recieve();
    if (request.Length > 0) 
    {
        RequestParser::HTTPRequestMap httpRequestMap = RequestParser::ParseRequest(request);
        HttpResponse response = { };
        
        response.Header = std::string("HTTP/1.1 200 OK\n");

        response.Body = std::string("<!DOCTYPE html>\n")            + 
                        std::string("<html>\n")                     +
                        std::string("<head> \n")                    +
                        std::string("   <meta charset='utf-8'>\n")  +
                        std::string("</head>\n")                    +
                        std::string("<body>\n")                     +
                        std::string("   <h1>Hello, World!</h1>\n")  +
                        std::string("</body>\n")                    +
                        std::string("</html>\n");

        std::string debugStr = "";

        if (httpRequestMap.find("GET") != httpRequestMap.end())
        {
            if (httpRequestMap["GET"] != "/" && httpRequestMap["GET"] != "/index.html")
            {
                std::string fileName = httpRequestMap["GET"];
                size_t slashLoc = fileName.find_first_of('/');
                
                if (slashLoc != std::string::npos)
                {
                    fileName.erase(0, slashLoc + 1);
                }

                response.Body.clear();
                FileUtils::LoadedFile loadedFile = FileUtils::ReadFile(fileName);
                response.Body = loadedFile.Data;
                response.BodyLength = loadedFile.Length;

                if (fileName.find(".css") != std::string::npos)
                {
                    response.Header += "Content-Type: text/css\n";
                }
                else if (fileName.find(".js") != std::string::npos)
                {
                    response.Header += "Content-Type: text/javascript\n";
                }
            }
            else 
            {
                response.Body.clear();
                FileUtils::LoadedFile loadedFile = FileUtils::ReadFile("index.html");
                response.Body = loadedFile.Data;
                response.BodyLength = loadedFile.Length;

                response.Header += "Content-Type: text/html\n";
            }
        }

        std::ostringstream s;
        s << "Content-Length: " << response.Body.length() << "\n\n";

        response.Header += s.str();

        while (!connectionFlags.at(std::this_thread::get_id())) 
        { 
            std::this_thread::yield();
        };

        int iSendResult = client.Send(response);

        if (iSendResult != SOCKET_ERROR)
        {
            printf("Sent %d bytes to %s\n\n", iSendResult, httpRequestMap.at("Host").c_str());
        }
    }
    else if (request.Length != 0) 
    {
        printf("recv failed with error: %d\n", WSAGetLastError());
    }

    client.Close();
}

ConnectionManager::ConnectionManager()
{
    _connectionThread = std::thread(&ConnectionThread, std::ref(_connections), std::ref(_connectionSendFlags), std::ref(_connectionMutex), std::ref(_running));
}

ConnectionManager::~ConnectionManager()
{
    Stop();
    _connectionThread.join();
}

void ConnectionManager::PushConnection(SOCKET client)
{
    HttpClient httpClient(client);
    bool shouldSend = false;

    std::unique_lock<std::mutex> lock(_connectionMutex);
    std::thread requestThread = std::thread(RequestHandler, httpClient, std::ref(_connectionSendFlags));
    _connectionSendFlags.emplace(requestThread.get_id(), shouldSend);
    _connections.push(std::move(requestThread));
    lock.unlock();
}

void ConnectionManager::Stop()
{
    _running = false;
}