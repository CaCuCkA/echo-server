#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <sys/socket.h>
#include <sstream>
#include <map>
#include <netinet/in.h>

namespace
{
    constexpr uint16_t kBufferSize = 1024;
    constexpr uint8_t SOCKET_ERROR = -1;
}

void Check(uint8_t expression, const std::string &message)
{
    if (expression == SOCKET_ERROR)
    {
        std::cerr << message << std::endl;
        exit(-1);
    }
}

void HandleClient(int clientSocket)
{
    char buffer[kBufferSize];
    ssize_t bytesRead;
    ssize_t bytesWritten;

    while (true)
    {
        bytesRead = recv(clientSocket, buffer, kBufferSize, 0);
        if (bytesRead == -1)
        {
            std::cerr << "Failed to read from client" << std::endl;
            break;
        }
        else if (bytesRead == 0)
        {
            std::cout << "Client closed the connection" << std::endl;
            break;
        }
        else
        {
            std::cout << "Read from client: " << std::string(buffer, bytesRead) << std::endl;

            bytesWritten = send(clientSocket, buffer, bytesRead, 0);
            if (bytesWritten == -1)
            {
                std::cerr << "Failed to write to client" << std::endl;
                break;
            }

            memset(buffer, 0, kBufferSize);
        }
    }

    std::cout << "Closing connection: " << clientSocket << std::endl;
    close(clientSocket);
}
#endif //UTILS_H
