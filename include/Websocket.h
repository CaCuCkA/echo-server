#ifndef ECHO_SERVER_WEBSOCKET_H
#define ECHO_SERVER_WEBSOCKET_H

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "CrossType.h"

#include <string>

class Websocket
{
public:
    Websocket(const Websocket& other) = delete;
    Websocket& operator=(const Websocket& other) = delete;
    Websocket(Websocket&& other) = delete;
    Websocket operator=(Websocket&& other) = delete;

    Websocket();
    explicit Websocket(const std::string& t_address, unsigned char t_port);

    ~Websocket();

    void Bind(const std::string& t_address, unsigned char t_port);
    void Listen();
    void Accept(Websocket& other);
private:
    static cross_types::address_type MakeAddress(const std::string& address, unsigned short port) noexcept;
private:
    cross_types::socket_type m_socket;
};
#endif //ECHO_SERVER_WEBSOCKET_H
