#include <iostream>
#include <cstring>

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
    WSADATA wsData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsData);
    if (iResult != 0)
    {
        std::cerr << "WSAStartup failed with error: " << iResult << std::endl;
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
#ifdef __linux__
        close(serverSocket);
#elif _WIN32
        WSACleanup();
#endif
        exit(1);
    }

    if (listen(serverSocket, kBacklog) == -1)
    {
        std::cerr << "Failed to listen on socket" << std::endl;
#ifdef __linux__
        close(serverSocket);
#elif _WIN32
        WSACleanup();
#endif
        exit(1);
    }

    std::cout << "Listening on port " << kPort << std::endl;

    while (true)
    {
        socket_type clientSocket = accept(serverSocket, nullptr, nullptr);
        if (clientSocket == -1)
        {
            std::cerr << "Failed to accept client connection" << std::endl;
            continue;
        }

        std::cout << "Accepted connection from client: " << clientSocket << std::endl;

        char buffer[1024];
        memset(buffer, 0, 1024);
        int bytesReceived = recv(clientSocket, buffer, 1024, 0);
        if (bytesReceived == -1)
        {
            std::cerr << "Failed to receive data from client" << std::endl;
#ifdef __linux__
            close(clientSocket);
#elif _WIN32
            closesocket(clientSocket);
#endif
            continue;
        }

        std::cout << "Received " << bytesReceived << " bytes of data: " << buffer << std::endl;

        int bytesSent = send(clientSocket, buffer, bytesReceived, 0);
        if (bytesSent == -1)
        {
            std::cerr << "Failed to send data to client" << std::endl;
#ifdef __linux__
            close(clientSocket);
#elif _WIN32
            closesocket(clientSocket);
#endif
            continue;
        }

        std::cout << "Sent " << bytesSent << " bytes of data: " << buffer << std::endl;

#ifdef __linux__
        close(clientSocket);
#elif _WIN32
        closesocket(clientSocket);
#endif
    }

#ifdef __linux__
    close(serverSocket);
#elif _WIN32
    closesocket(serverSocket);
#endif

#ifdef _WIN32
    WSACleanup();
#endif

    return 0;
}
