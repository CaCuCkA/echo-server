#include "AsyncEchoServer.h"

AsyncEchoServer::AsyncEchoServer(std::string &&t_address, uint16_t t_port)
{
    cross_types::address_type serverAddress = MakeAddress(std::move(t_address), t_port);
    Create(m_socket);
    MakeAddressReused();
    Bind(m_socket, serverAddress);
    Listen(m_socket);
}

AsyncEchoServer::~AsyncEchoServer() noexcept
{
    CLOSE_SOCKET(m_socket);
}

void AsyncEchoServer::MakeAddressReused()
{
    int optionValue = 1;
    SetSocketOptions(m_socket, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<char *>(&optionValue), sizeof(int));
}

void AsyncEchoServer::CleanBuffer()
{
    memset(m_buffer, 0, BUFFER_SIZE);
}

void AsyncEchoServer::Run()
{
    cross_types::socket_type clientSocket;
    cross_types::recv_type bytesRecv;
    while (true)
    {
        FD_ZERO(&m_socketsSet);
        FD_SET(m_socket, &m_socketsSet);
        try
        {
            Select(m_socket, &m_socketsSet);

            if (FD_ISSET(m_socket, &m_socketsSet))
            {
                Accept(clientSocket, m_socket);
                CleanBuffer();
                bytesRecv = Read(clientSocket, m_buffer, BUFFER_SIZE);
                Send(clientSocket, m_buffer, bytesRecv);
            }
        }
        catch (...)
        {
            CLOSE_SOCKET(clientSocket);
            break;
        }
        CLOSE_SOCKET(clientSocket);
    }
}
