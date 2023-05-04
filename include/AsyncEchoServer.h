#ifndef ECHO_SERVER_ASYNC_ECHO_SERVER_H
#define ECHO_SERVER_ASYNC_ECHO_SERVER_H

#include  <boost/asio.hpp>
#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "Server.h"
#include "CrossType.h"

#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio.hpp>
#include <boost/asio/strand.hpp>
using cross_types::socket_type;

class AsyncEchoServer : public Server
{
public:
    AsyncEchoServer() = delete;
    AsyncEchoServer(const AsyncEchoServer&) = delete;
    AsyncEchoServer& operator=(const AsyncEchoServer&) = delete;
    AsyncEchoServer(AsyncEchoServer&&) = delete;
    AsyncEchoServer& operator=(AsyncEchoServer&&) = delete;

    AsyncEchoServer(std::string&& t_address, uint16_t t_port);
    ~AsyncEchoServer() override;

    void Run() override;

private:
    void MakeAddressReused();
    void CleanBuffer();

private:
    socket_type m_socket;
    fd_set m_socketsSet;
    char m_buffer[BUFFER_SIZE];
};

#endif // ECHO_SERVER_ASYNC_ECHO_SERVER_H