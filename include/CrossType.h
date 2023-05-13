#ifndef ECHO_SERVER_CROSS_TYPE_H
#define ECHO_SERVER_CROSS_TYPE_H

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif

#if defined(_WIN32) || defined(WIN32)
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#elif defined(__linux__)
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#else
#error "Cannot detect OS"
#endif

#ifdef TBB_LIB
#include <tbb/tbb.h>
#else
#include "ThreadSafeQueue.h"
#endif

namespace cross_types
{
#if defined(_WIN32) || defined(WIN32)
    typedef SOCKET socket_type;
    typedef SOCKADDR_IN address_type;
    typedef SOCKADDR socket_address;
    typedef int address_len;
    typedef int recv_type;
    typedef int send_type;
    typedef char* option_type;
#define NON_BLOCK 1
#define WIN_FLAG FIONBIO
#else
    typedef int socket_type;
    typedef sockaddr_in address_type;
    typedef sockaddr socket_address;
    typedef socklen_t address_len;
    typedef ssize_t recv_type;
    typedef ssize_t send_type;
    typedef const void* option_type;
#define NON_BLOCK O_NONBLOCK
#define WIN_FLAG -1
#endif // defined(_WIN32) || defined(WIN32)

#ifdef TBB_LIB
    template <typename ValueType>
    using queue_type =  tbb::concurrent_bounded_queue<ValueType>;
#else
    template <typename ValueType>
    using queue_type =  ThreadSafeQueue<ValueType>;
#endif
}
#endif //ECHO_SERVER_CROSS_TYPE_H
