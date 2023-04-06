#include "Utils.h"

#include <iostream>
#include <vector>
#include <thread>
#ifdef __linux__
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
using socket_type = int;
using address_type = sockaddr_in;
#elif _WIN32
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
using socket_type = SOCKET;
using address_type = SOCKADDR_IN;
#endif

namespace
{
    constexpr uint16_t kPort = 12345;
    constexpr int kBacklog = 5;
}

int main()
{
#ifdef _WIN32
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0)
    {
        std::cerr << "WSAStartup failed with error: " << result << std::endl;
        return 1;
    }
#endif

    socket_type serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1)
    {
        std::cerr << "Failed to create socket" << std::endl;
        return 1;
    }

    address_type address{};
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &address.sin_addr);
    address.sin_port = htons(kPort);

    if (bind(serverSocket, (sockaddr*)&address, sizeof(address)) == -1)
    {
        std::cerr << "Failed to bind socket" << std::endl;
#ifdef _WIN32
        closesocket(serverSocket);
#elif __linux__
        close(serverSocket);
#endif
        exit(1);
    }

    if (listen(serverSocket, SOMAXCONN) == -1)
    {
        std::cerr << "Failed to listen on socket" << std::endl;
#ifdef _WIN32
        closesocket(serverSocket);
#elif __linux__
        close(serverSocket);
#endif
        exit(1);
    }

    std::cout << "Listening on port " << kPort << std::endl;

    std::vector<std::thread> threads;

    while (true)
    {
        socket_type clientSocket = accept(serverSocket, nullptr, nullptr);
        if (clientSocket == -1)
        {
            std::cerr << "Failed to accept client connection" << std::endl;
            continue;
        }

        std::cout << "Accepted connection from client: " << clientSocket << std::endl;

        threads.emplace_back(HandleClient, clientSocket);
    }

#ifdef _WIN32
    closesocket(serverSocket);
    WSACleanup();
#elif __linux__
    close(serverSocket);
#endif

    return 0;
}
