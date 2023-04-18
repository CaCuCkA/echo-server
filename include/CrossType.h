#ifndef ECHO_SERVER_CROSS_TYPE_H
#define ECHO_SERVER_CROSS_TYPE_H

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#if defined(_WIN32) || defined(WIN32)
#include <windows.h>
#include <ws2tcpip.h>
#include <winsock2.h>
#pragma comment(lib, "Ws2_32.lib")
#elif defined(__linux__)
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#else
#error "Cannot detect OS"
#endif // detect os for including libs

namespace cross_types
{
#if defined(_WIN32) || defined(WIN32)
    typedef SOCKET socket_type;
    typedef SOCKADDR_IN address_type;
    typedef SOCKADDR socket_address;
#define CLOSE_SOCKET(socket) do \
{                               \
    closesocket(socket)         \
    WSACleanup();               \
} while (0)
#else
    typedef int socket_type;
    typedef sockaddr_in address_type;
    typedef sockaddr socket_address;
#define CLOSE_SOCKET(socket) close(socket)
#endif // defined(_WIN32) || defined(WIN32)
}
#endif //ECHO_SERVER_CROSS_TYPE_H
