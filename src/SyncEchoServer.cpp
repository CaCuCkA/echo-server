#include "SyncEchoServer.h"


SyncEchoServer::SyncEchoServer(std::string &&t_address, uint16_t t_port)
{
    cross_types::address_type serverAddress = MakeAddress(std::move(t_address), t_port);
    Create(m_socket);
    Bind(m_socket, serverAddress);
    Listen(m_socket);
}

SyncEchoServer::~SyncEchoServer() noexcept
{
    CLOSE_SOCKET(m_socket);
}

void SyncEchoServer::Run()
{
    char buffer[BUFFER_SIZE];
    cross_types::recv_type bytesRecv;
    cross_types::socket_type clientSocket;

    while(true)
    {
        try
        {
            Accept(clientSocket, m_socket);
            bytesRecv = Read(clientSocket, buffer, BUFFER_SIZE);
            Send(clientSocket, buffer, bytesRecv);
        }
        catch(...)
        {
            CLOSE_SOCKET(clientSocket);
            break;
        }
        CLOSE_SOCKET(clientSocket);
    }
}

