#ifndef ECHO_SERVER_CROSS_FUNCTIONS_H
#define ECHO_SERVER_CROSS_FUNCTIONS_H

#include "CrossType.h"
#include "Exception.h"

#if defined(_WIN32) || defined(WIN32)

#include <winsock2.h>
#define CLOSE_SOCKET(socket) closesocket(socket)

template <typename SocketType, typename ...Args>
inline void SetSocketFlag(SocketType& t_socket, Args... args)
{
    if (sizeof...(args) < 2)
    {
        throw EXCEPTION(EC_NOT_ENOUGH_ARGS, "Failed to get arguments");
    }
     auto params = std::make_tuple(args...);
    u_long win_flag = std::get<0>(params);
    if (ioctlsocket(t_socket, std::get<1>(params), &win_flag) == SOCKET_ERROR)
    {
        throw EXCEPTION(EC_CANT_SET_FLAG_TO_SOCKET, "Failed to set socket flag");
    }
}
#else
#define CLOSE_SOCKET(socket) close(socket)

template <typename SocketType, typename ...Args>
inline void SetSocketFlag(SocketType& t_socket, Args... args)
{
    if (sizeof...(args) < 1)
    {
        throw EXCEPTION(EC_NOT_ENOUGH_ARGS, "Failed to get arguments");
    }

    auto params = std::make_tuple(args...);
    int current_flags = fcntl(t_socket, F_GETFL, 0);
    if (current_flags == -1)
    {
        throw EXCEPTION(EC_CANT_SET_FLAG, "Failed to get current flags");
    }

    if (fcntl(t_socket, F_SETFL, current_flags | std::get<0>(params)) == -1)
    {
        throw EXCEPTION(EC_CANT_SET_FLAG_TO_SOCKET, "Failed to set socket flag");
    }
}
#endif

#endif //ECHO_SERVER_CROSS_FUNCTIONS_H
