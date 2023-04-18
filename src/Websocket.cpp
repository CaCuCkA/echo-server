#include "Websocket.h"
#include "Exception.h"

#include <cstring>

Websocket::Websocket()
{
    std::string errorMsg;
    m_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (m_socket == EC_CANT_CREATE_SOCKET)
    {
        auto msg = "Can`t create socket";
        EXCEPTION(EC_CANT_CREATE_SOCKET, msg);
    }
}

Websocket::Websocket(const std::string &t_address, unsigned char t_port)
{
    Websocket websocket;
    websocket.Bind(t_address, t_port);
    m_socket = websocket.m_socket;
}

Websocket::~Websocket()
{
    CLOSE_SOCKET(m_socket);
}

cross_types::address_type Websocket::MakeAddress(const std::string& t_address, unsigned short port) noexcept
{
    cross_types::address_type address{};
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    inet_pton(AF_INET, t_address.c_str(), &address.sin_port);
    address.sin_port = htons(port);
    return address;
}

void Websocket::Bind(const std::string& t_address, unsigned char t_port)
{
    cross_types::address_type address = MakeAddress(t_address, t_port);

    auto error = bind(m_socket,
                      reinterpret_cast<cross_types::socket_address*>(&address),
                      sizeof(address));

    if (error == EC_CANT_BIND_SOCKET)
    {
        auto msg = "Can`t bind socket";
        EXCEPTION(EC_CANT_BIND_SOCKET, msg);
    }
}

void Websocket::Listen()
{
    auto error = listen(m_socket, SOMAXCONN);

    if (error == EC_CANT_LISTEN_SOCKET)
    {
        auto msg = "Failed to listen on socket";
        EXCEPTION(EC_CANT_LISTEN_SOCKET, msg);
    }
}

void Websocket::Accept(Websocket& other)
{
    auto error = accept(m_socket, reinterpret_cast<cross_types::socket_address *>(&other.m_socket),
                        reinterpret_cast<socklen_t *>(sizeof(&other.m_socket)));

    if (error == EC_CANT_ACCEPT_SOCKET)
    {
        auto msg = "Failed to accept socket";
        EXCEPTION(EC_CANT_ACCEPT_SOCKET, msg);
    }
}
