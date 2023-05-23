#ifndef ECHO_SERVER_SERVER_H
#define ECHO_SERVER_SERVER_H

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <string>
#include <functional>
#include <boost/beast/http.hpp>
#include <boost/beast/core/flat_buffer.hpp>

#include "Utils.h"
#include "CrossType.h"
#include "CrossFunctions.h"
#include "Exception.h"

#define BUFFER_SIZE 1028

using cross_types::socket_type;

namespace http = boost::beast::http;

class Server
{
public:
    Server() = default;
    virtual ~Server() = default;
    Server(const Server&) = delete;
    Server& operator=(const Server&) = delete;
    Server(Server&&) = delete;
    Server& operator=(Server&&) = delete;

    virtual void Run() = 0;
    static void CleanBuffer(char* buffer);
    static http::fields ParseHeaders(const char* header);
    static void  ConnectionWithHandshake(cross_types::socket_type& t_socket, const http::fields& fields);
    static void HandleClientConnection(int t_socket);
    static void SetSocketNonblocking(cross_types::socket_type& t_socket);

protected:
    static void MakeAddressReused(socket_type& t_socket);
    static cross_types::address_type MakeAddress(std::string&& t_address, uint16_t t_port);
    static void Create(cross_types::socket_type& t_socket);
    static void Bind(cross_types::socket_type& t_socket, cross_types::address_type& t_address);
    static void Listen(cross_types::socket_type& t_socket);
    static void EpollCtlAdd(int epollFileDesc, int t_socket, uint32_t events);

    static void
    SetSocketOptions(cross_types::socket_type& t_socket, int level, int socketOption, cross_types::option_type buffer,
                     size_t bufferSize);

    static void Accept(cross_types::socket_type& t_socket, cross_types::socket_type& listeningSocket,
                cross_types::socket_address* socketAddress, cross_types::address_len* addressLength);
    static void Accept(cross_types::socket_type& t_socket, cross_types::socket_type& listeningSocket);

    static cross_types::recv_type Read(cross_types::socket_type& t_socket, char* buffer, uint16_t bufferLength);

    static cross_types::send_type
    Send(cross_types::socket_type& t_socket, char* buffer, cross_types::recv_type bytesRecv, int flag = 0);
    static void SendWebsocketFrame(cross_types::socket_type &t_socket, std::string &payload, size_t payloadLength);

    template <typename ...Args>
    static int Fcntl(cross_types::socket_type& t_socket, int cmd, Args... args)
    {
        if (fcntl(t_socket, cmd, args...) == EC_CANT_SET_SOCKET_OPTION)
        {
            throw EXCEPTION(EC_CANT_SET_SOCKET_OPTION, "Failed to set socket option");
        }
        return 0;
    }

    friend class Handshake;

};
#endif //ECHO_SERVER_SERVER_H