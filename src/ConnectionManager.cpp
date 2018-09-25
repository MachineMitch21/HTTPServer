
#include "ConnectionManager.hpp"

#include <sstream>

#include "RequestParser.hpp"
#include "FileUtils.hpp"
#include "HttpClient.hpp"
#include "HttpRequest.hpp"

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

void RequestHandler(HttpClient client)
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

        printf("HTTPResponse body length: %d\n", response.Body.length());

        std::ostringstream s;
        s << "Content-Length: " << response.Body.length() << "\n\n";

        response.Header += s.str();

        int sendResult = client.Send(response);

        if (sendResult == SOCKET_ERROR)
        {
            printf("client.Send failed: ERROR #%d\n", WSAGetLastError());
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
    _connections.push(std::move(std::thread(RequestHandler, HttpClient(client))));
    lock.unlock();
    _conditionVar.notify_one();
}

void ConnectionManager::Stop()
{
    _running = false;
}