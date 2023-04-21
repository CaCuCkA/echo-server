#include "Server.h"
#include "CrossType.h"
#include "Exception.h"

#include <utility>
#include <cstring>
#include <iostream>

Server::Server(std::string t_address, uint16_t t_port,
               const std::function<void(cross_types::socket_type&)>& t_function)
    :
    m_address(std::move(t_address)), m_port(t_port), m_function(t_function)
{}


cross_types::address_type Server::MakeAddress()
{
    cross_types::address_type address{};
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    inet_pton(AF_INET, m_address.c_str(), &address.sin_addr);
    address.sin_port = htons(m_port);
    return address;
}

void Server::Create(cross_types::socket_type& t_socket)
{
    t_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (t_socket == EC_CANT_CREATE_SOCKET)
    {
        throw EXCEPTION(EC_CANT_CREATE_SOCKET, "Can`t create socket");
    }
}

void Server::Bind(cross_types::socket_type &t_socket)
{
    cross_types::address_type address = MakeAddress();

    auto error = bind(t_socket,
                      reinterpret_cast<cross_types::socket_address*>(&address),
                      sizeof(address));
    if(error == EC_CANT_BIND_SOCKET)
    {
        throw EXCEPTION(EC_CANT_BIND_SOCKET, "Can`t bind socket");
    }
}

void Server::Listen(cross_types::socket_type &t_socket)
{
    auto error = listen(t_socket, SOMAXCONN);
    if (error == EC_CANT_LISTEN_SOCKET)
    {
        throw EXCEPTION(EC_CANT_LISTEN_SOCKET, "Failed to listen socket");
    }
}

void Server::Run()
{
    cross_types::socket_type serverSocket{};
    Create(serverSocket);
    Bind(serverSocket);
    Listen(serverSocket);
    m_function(serverSocket);
}
