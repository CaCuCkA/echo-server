#ifndef ECHO_SERVER_SERVER_H
#define ECHO_SERVER_SERVER_H

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "Utils.h"
#include "CrossType.h"
#include "CrossFunctions.h"
#include "Exception.h"

#include <string>
#include <functional>

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

protected:
    static cross_types::address_type MakeAddress(std::string&& t_address, uint16_t t_port);
    static void Create(cross_types::socket_type& t_socket);
    static void Bind(cross_types::socket_type& t_socket, cross_types::address_type& t_address);
    static void Listen(cross_types::socket_type& t_socket);
    static void MakeSocketNonBlocking(cross_types::socket_type& t_socket);
    static void Select();

    static void Accept(cross_types::socket_type& t_socket, cross_types::socket_type& listeningSocket,
                cross_types::socket_address* socketAddress, cross_types::address_len* addressLength);
    static void Accept(cross_types::socket_type& t_socket, cross_types::socket_type& listeningSocket);
    static cross_types::socket_type
    Accept(cross_types::socket_type& listeningSocket, cross_types::socket_address* socketAddress,
                cross_types::address_len* addressLength);

    static cross_types::recv_type Read(cross_types::socket_type& t_socket, char* buffer, uint16_t bufferLength);
    static cross_types::send_type
    Send(cross_types::socket_type& t_socket, char* buffer, cross_types::recv_type& bytesRecv, int flag = 0);
};
#endif //ECHO_SERVER_SERVER_H
