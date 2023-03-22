#include "StaticThreadPool.h"
#include "Utils.h"

#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
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
    serverAddress.sin_port= htons(54000);

    expression = bind(serverSocket, reinterpret_cast<SA*>(&serverAddress), sizeof(serverAddress));
    Check(expression, "Can`t bind to IP/port");

    expression = listen(serverSocket, SOMAXCONN);
    Check(expression, "Can`t listen");
    
    // TODO
    while (true)
    {
        std::cout << "Waiting for connections..." << std::endl;

        addressSize = sizeof(SA_IN);
        clientSocket = accept(serverSocket, reinterpret_cast<SA*>(&clientAddress),
                              reinterpret_cast<socklen_t*>(&addressSize));
        Check(clientSocket, "Problem with client connecting");
        std::cout << "Connected!" << std::endl;

        HandleConnection(clientSocket);
    }
    return 0;
}
