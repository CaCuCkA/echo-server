#include "StaticThreadPool.h"
#include "Utils.h"

#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string>
#include <cstring>

typedef sockaddr_in SA_IN;
typedef sockaddr SA;

int main(int args, char* argv[])
{
    uint16_t serverSocket;
    uint16_t clientSocket;
    int addressSize;
    uint8_t expression;

    SA_IN serverAddress;
    SA_IN clientAddress;

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    Check(serverSocket, "Can`t create a socket!");

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port= htons(8083);

    expression = bind(serverSocket, reinterpret_cast<SA*>(&serverAddress), sizeof(serverAddress));
    Check(expression, "Can`t bind to IP/port");

    expression = listen(serverSocket, SOMAXCONN);
    Check(expression, "Can`t listen");

    auto task = [](int clientSocket)
    {
        char buffer[4096];
        while (true)
        {
            memset(buffer, 0, 4096);

            int bytesReceive = recv(clientSocket, buffer, 4096, 0);

            if (bytesReceive == -1)
            {
                std::cerr << "There was a connect issue" << std::endl;
                break;
            }

            if (bytesReceive == 0)
            {
                std::cout << "The client disconnected" << std::endl;
                break;
            }

            std::cout << "Received: " << std::string(buffer, 0, bytesReceive) << std::endl;

            send(clientSocket, buffer, bytesReceive + 1, 0);
        }

        close(clientSocket);
    };

    StaticThreadPool threadPool;

    while (true)
    {
        socklen_t clientSize = sizeof(clientAddress);
        clientSocket = accept(serverSocket, reinterpret_cast<SA *>(&clientAddress), &clientSize);

        Check(clientSocket, "Problem with client connecting");
        std::cout << "Connected " << inet_ntoa(clientAddress.sin_addr) << ":" << ntohs(clientAddress.sin_port)
                << ", socket: " << clientSocket << std::endl;

       threadPool.Submit([=]()
                          {
                                return task(clientSocket);
                          });
    }
    return 0;
}
