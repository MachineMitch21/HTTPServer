
#include "HttpClient.hpp"

HttpClient::HttpClient(SOCKET clientConnection)
    :   _clientConnection(clientConnection)
{

}

HttpClient::~HttpClient()
{

}

int HttpClient::Send(HttpResponse response)
{
    std::string sendStr = response.Header + response.Body;
    int iSendResult = send(_clientConnection, sendStr.c_str(), response.Header.length() + response.BodyLength, 0);

    if (iSendResult == SOCKET_ERROR)
    {
        printf("Failed to send HttpResponse: ERROR #%d\n", iSendResult);
    }
    else 
    {
        printf("Sent HttpResponse with %d bytes\n", iSendResult);
    }

    return iSendResult;
}

HttpRequest HttpClient::Recieve()
{
    char recvBuffer[1024];
    int iRecvResult = recv(_clientConnection, recvBuffer, 1024, 0);

    return HttpRequest { std::string(recvBuffer), iRecvResult };
}

void HttpClient::Close()
{
    closesocket(_clientConnection);
}
