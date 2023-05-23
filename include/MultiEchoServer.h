#ifndef ECHO_SERVER_MULTI_ECHO_SERVER_H
#define ECHO_SERVER_MULTI_ECHO_SERVER_H

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "Server.h"
#include "CrossType.h"

#include <string>

using cross_types::socket_type;

class MultiEchoServer : public Server
{
public:
    MultiEchoServer() = delete;
    MultiEchoServer(const MultiEchoServer&) = delete;
    MultiEchoServer& operator=(const MultiEchoServer&)= delete;
    MultiEchoServer(MultiEchoServer&&) = delete;
    MultiEchoServer& operator=(MultiEchoServer&&) = delete;

    MultiEchoServer(std::string&& t_address, uint16_t t_port, uint8_t t_amountOfThreads);
    ~MultiEchoServer() override;

    static MultiEchoServer& Get(std::string&& t_address, uint16_t t_port, uint8_t t_amountOfThreads)
    {
        static MultiEchoServer s_instance(std::move(t_address), t_port, t_amountOfThreads);
        return s_instance;
    }

    void Run() override;

private:
    uint8_t m_amountOfThreads;
    socket_type m_socket{};
};
#endif //ECHO_SERVER_MULTI_ECHO_SERVER_H
