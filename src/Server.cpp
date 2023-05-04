#include <cstring>
#include <iostream>

#include "Server.h"
#include "CrossType.h"
#include "Exception.h"

cross_types::address_type Server::MakeAddress(std::string &&t_address, uint16_t t_port)
{
    cross_types::address_type address{};
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    inet_pton(AF_INET, t_address.c_str(), &address.sin_addr);
    address.sin_port = htons(t_port);
    return address;
}

void Server::Create(cross_types::socket_type &t_socket)
{
    t_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (t_socket == EC_CANT_CREATE_SOCKET)
    {
        CLOSE_SOCKET(t_socket);
        throw EXCEPTION(EC_CANT_CREATE_SOCKET, "Can`t create socket");
    }
}

void Server::Bind(cross_types::socket_type &t_socket, cross_types::address_type &t_address)
{
    auto error = bind(t_socket,
                      reinterpret_cast<cross_types::socket_address *>(&t_address),
                      sizeof(t_address));
    if (error == EC_CANT_BIND_SOCKET)
    {
        CLOSE_SOCKET(t_socket);
        throw EXCEPTION(EC_CANT_BIND_SOCKET, "Can`t bind socket");
    }
}

void Server::Listen(cross_types::socket_type &t_socket)
{
    auto error = listen(t_socket, SOMAXCONN);
    if (error == EC_CANT_LISTEN_SOCKET)
    {
        CLOSE_SOCKET(t_socket);
        throw EXCEPTION(EC_CANT_LISTEN_SOCKET, "Failed to listen socket");
    }
}

void Server::Select(cross_types::socket_type& t_socket, fd_set* readfds, fd_set* writefds, fd_set* exceptionfds,
                    timeval* timeout)
{
    auto error = select(t_socket + 1, readfds, writefds, exceptionfds, timeout);
    if (error == EC_CANT_SELECT_SOCKET)
    {
        if (errno == EINTR || errno == EWOULDBLOCK || errno == EAGAIN)
        {
            Select(t_socket, readfds, writefds, exceptionfds, timeout);
            return;
        }
        throw EXCEPTION(EC_CANT_SELECT_SOCKET, "Failed to select socket");
    }
}

void Server::SetSocketOptions(cross_types::socket_type &t_socket, int level, int socketOption, cross_types::option_type buffer,
                              size_t bufferSize)
{
    auto error = setsockopt(t_socket, level, socketOption, buffer, bufferSize);
    if (error == EC_CANT_SET_SOCKET_OPTION)
    {
        CLOSE_SOCKET(t_socket);
        throw EXCEPTION(EC_CANT_SET_SOCKET_OPTION, "Failed to set socket option");
    }
}

void Server::Accept(cross_types::socket_type &t_socket, cross_types::socket_type &listeningSocket,
                    cross_types::socket_address *socketAddress, cross_types::address_len *addressLength)
{
    t_socket = accept(listeningSocket, socketAddress, addressLength);
    if (t_socket == EC_CANT_ACCEPT_SOCKET)
    {
        throw EXCEPTION(EC_CANT_LISTEN_SOCKET, "Failed to listen socket");
    }
}

void Server::Accept(cross_types::socket_type& t_socket, cross_types::socket_type& listeningSocket)
{
    Accept(t_socket, listeningSocket, nullptr, nullptr);
}

cross_types::socket_type
Server::Accept(cross_types::socket_type &listeningSocket, cross_types::socket_address* socketAddress,
               cross_types::address_len* addressLength)
{
    cross_types::socket_type acceptingSocket;
    Accept(acceptingSocket, listeningSocket, socketAddress, addressLength);
    return acceptingSocket;
}

cross_types::recv_type Server::Read(cross_types::socket_type &t_socket, char *buffer, uint16_t bufferLength)
{
    cross_types::recv_type bytesRecv = recv(t_socket, buffer, bufferLength, 0);
    if (bytesRecv == EC_CANT_READ_DATA)
    {
        CLOSE_SOCKET(t_socket);
        throw EXCEPTION(EC_CANT_READ_DATA, "Failed to read data from socket");
    }
    return bytesRecv;
}

cross_types::send_type
Server::Send(cross_types::socket_type &t_socket, char *buffer, cross_types::recv_type &bytesRecv, int flag)
{
    auto sendBytes = send(t_socket, buffer, bytesRecv, flag);
    if (sendBytes == EC_CANT_SEND_DATA)
    {
        CLOSE_SOCKET(t_socket);
        throw EXCEPTION(EC_CANT_SEND_DATA, "Failed to send data from socket");
    }
    return sendBytes;
}
