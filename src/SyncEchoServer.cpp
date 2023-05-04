#include <cstring>
#include "SyncEchoServer.h"


SyncEchoServer::SyncEchoServer(std::string &&t_address, uint16_t t_port)
{
    cross_types::address_type serverAddress = MakeAddress(std::move(t_address), t_port);
    Create(m_socket);
    MakeAddressReused();
    Bind(m_socket, serverAddress);
    Listen(m_socket);
}

SyncEchoServer::~SyncEchoServer() noexcept
{
    CLOSE_SOCKET(m_socket);
}

void SyncEchoServer::MakeAddressReused()
{
    int optionValue = 1;
    SetSocketOptions(m_socket, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<char *>(&optionValue), sizeof(int));
}

void SyncEchoServer::CleanBuffer()
{
    memset(m_buffer, 0, BUFFER_SIZE);
}

void SyncEchoServer::Run()
{
    cross_types::recv_type bytesRecv;
    cross_types::socket_type clientSocket;

    while(true)
    {
        try
        {
            Accept(clientSocket, m_socket);
            CleanBuffer();
            bytesRecv = Read(clientSocket, m_buffer, BUFFER_SIZE);
            Send(clientSocket, m_buffer, bytesRecv);
        }
        catch(...)
        {
            CLOSE_SOCKET(clientSocket);
            break;
        }
        CLOSE_SOCKET(clientSocket);
    }

}

