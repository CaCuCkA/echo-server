#ifndef ECHO_SERVER_SERVER_H
#define ECHO_SERVER_SERVER_H

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "Base.h"
#include "CrossType.h"

#include <string>
#include <functional>


class Server : Base
{
public:
    Server() = delete;
    Server(const Server&) = delete;
    Server& operator=(const Server&) = delete;
    Server(Server&&) = delete;
    Server& operator=(Server&&) = delete;

    Server(std::string  t_address, uint16_t t_port, const std::function<void(cross_types::socket_type&)>& t_function);
    ~Server() override = default;

    void Run();

private:
    cross_types::address_type MakeAddress() override;

    void Create(cross_types::socket_type& socket) override;
    void Bind(cross_types::socket_type& socket) override;
    void Listen(cross_types::socket_type& socket) override;

private:
    std::string m_address;
    uint16_t m_port;
    std::function<void(cross_types::socket_type&)> m_function;
};

#endif //ECHO_SERVER_SERVER_H
