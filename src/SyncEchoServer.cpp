#include <iostream>

#include "Handshake.h"
#include "SyncEchoServer.h"


SyncEchoServer::SyncEchoServer(std::string &&t_address, uint16_t t_port)
{
    cross_types::address_type serverAddress = MakeAddress(std::move(t_address), t_port);
    Create(m_socket);
    MakeAddressReused(m_socket);
    Bind(m_socket, serverAddress);
    Listen(m_socket);
}

SyncEchoServer::~SyncEchoServer()
{
    CLOSE_SOCKET(m_socket);
}

void SyncEchoServer::Run()
{
    cross_types::socket_type clientSocket;

    while (true)
    {
        try
        {
            Accept(clientSocket, m_socket);
            HandleClientConnection(clientSocket);
        }
        catch(...)
        {
            CLOSE_SOCKET(clientSocket);
            break;
        }
    }
}

