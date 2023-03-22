#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <sys/socket.h>

namespace
{
    constexpr uint8_t SOCKET_ERROR = -1;
    constexpr uint16_t BUFFER_SIZE = 4096;
}

void Check(uint8_t expression, const std::string& message)
{
    if (expression == SOCKET_ERROR)
    {
        std::cerr << message << std::endl;
        exit(-1);
    }
}


int HandleConnection(uint16_t clientSocket)
{
    char buffer[BUFFER_SIZE];
    size_t bytesReceive;

    memset(buffer, 0, BUFFER_SIZE);

    bytesReceive = recv(clientSocket, buffer, BUFFER_SIZE, 0);
    Check(bytesReceive, "There was a connect issue");

    if (bytesReceive == 0)
    {
        std::cout << "The client disconnected" << std::endl;
        return 0;
    }

    std::cout << "Received: " << std::string(buffer, 0, bytesReceive) << std::endl;

    send(clientSocket, buffer, bytesReceive + 1, 0);
}
#endif //UTILS_H
