#ifndef ECHO_SERVER_SERVER_H
#define ECHO_SERVER_SERVER_H

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "CrossType.h"
#include "Exception.h"

#include <string>
#include <functional>

class Server
{
public:
    Server() = default;
    ~Server() = default;
    Server(const Server&) = delete;
    Server& operator=(const Server&) = delete;
    Server(Server&&) = delete;
    Server& operator=(Server&&) = delete;

    virtual void Run() = 0;

private:
    static cross_types::address_type MakeAddress(std::string&& t_address, uint16_t t_port);
    void Create(cross_types::socket_type& t_socket);
    void Bind(cross_types::socket_type& t_socket, cross_types::address_type& t_address);
    void Listen(cross_types::socket_type& t_socket);

    void Accept(cross_types::socket_type& t_socket, cross_types::socket_type& listeningSocket,
                cross_types::socket_address* socketAddress, cross_types::address_len* addressLength);
    void Accept(cross_types::socket_type& t_socket, cross_types::socket_type& listeningSocket);
    cross_types::socket_type
    Accept(cross_types::socket_type& listeningSocket, cross_types::socket_address* socketAddress,
                cross_types::address_len* addressLength);

    cross_types::recv_type Read(cross_types::socket_type& t_socket, char* buffer, uint16_t bufferLength);
    void Send(cross_types::socket_type& t_socket, char* buffer, cross_types::recv_type& bytesRecv);

private:
    cross_types::socket_type m_socket;
};
#endif //ECHO_SERVER_SERVER_H
