#ifndef ECHO_SERVER_SYNC_SERVER_H
#define ECHO_SERVER_SYNC_SERVER_H

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "Server.h"
#include "CrossType.h"

#include <string>

using cross_types::socket_type;

class SyncEchoServer : public Server
{
public:
    SyncEchoServer() = delete;
    SyncEchoServer(const SyncEchoServer&) = delete;
    SyncEchoServer& operator=(const SyncEchoServer&) = delete;
    SyncEchoServer(SyncEchoServer&&) = delete;
    SyncEchoServer& operator=(SyncEchoServer&&) = delete;

    SyncEchoServer(std::string&& t_address, uint16_t t_port);
    ~SyncEchoServer() override;

    static SyncEchoServer& Get(std::string&& t_address, uint16_t t_port)
    {
        static SyncEchoServer s_instance(std::move(t_address), t_port);
        return s_instance;
    }

    void Run() override;
private:
    void MakeAddressReused();
    void CleanBuffer();
private:
    socket_type m_socket;
    char m_buffer[BUFFER_SIZE];
};
#endif // ECHO_SERVER_SYNC_SERVER_H