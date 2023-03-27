#include "Utils.h"
#include "TCPHandshaker.h"

#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string>
#include <cstring>


int main(int args, char* argv[])
{
    int listening = socket(AF_INET, SOCK_STREAM, 0);

    Check(listening, "Can`t create a socket!");

    sockaddr_in hint{};
    hint.sin_family = AF_INET;
    hint.sin_port = htons(8083);
    inet_pton(AF_INET, "127.0.0.1", &hint.sin_addr);

    uint8_t expression = bind(listening, reinterpret_cast<const sockaddr *>(&hint), sizeof(hint));
    Check(expression, "Can`t bind to IP/port");

    expression =listen(listening, SOMAXCONN);
    Check(expression, "Can`t listen");

    sockaddr_in client{};
    socklen_t clientSize = sizeof(client);
    char host[NI_MAXHOST];
    char service[NI_MAXSERV];

    int clientSocket = accept(listening,
                              reinterpret_cast<sockaddr *>(&client),
                              &clientSize);

    Check(clientSocket, "Problem with client connecting");

//    close(listening);

    int result = getnameinfo(reinterpret_cast<sockaddr *>(&client),
                             sizeof(client),
                             host,
                             NI_MAXHOST,
                             service,
                             NI_MAXSERV,
                             0);


    if (result)
    {
        std::cout << host << " connected on service " << service << std::endl;
    }
    else
    {
        inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
        std::cout << host << " connected on " << ntohs(client.sin_port) << std::endl;
    }

    char buffer[4096];

    memset(buffer, 0, 4096);

    size_t bytesReceive = recv(clientSocket, buffer, 4096, 0);

    TCPHandshaker tcpHandshaker(static_cast<int>(bytesReceive), buffer);
    result = tcpHandshaker.PerformWebsocketHandshake(clientSocket);

    Check(result, "Failed to send WebSocket upgrade response");

    while (true)
    {
        memset(buffer, 0, 4096);

        bytesReceive = recv(clientSocket, buffer, 4096, 0);

        Check(static_cast<int>(bytesReceive), "There was a connect issue");

        if (bytesReceive == 0)
        {
            std::cout << "The client disconnected" << std::endl;
            break;
        }

        std::cout << "Received: " << std::string(buffer, 0, bytesReceive) << std::endl;

        send(clientSocket, buffer, bytesReceive + 1, 0);
    }

    close(clientSocket);

    return 0;
}
