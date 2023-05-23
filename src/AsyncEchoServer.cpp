#include <iostream>

#include "AsyncEchoServer.h"

AsyncEchoServer::AsyncEchoServer(std::string &&t_address, uint16_t t_port)
{
    cross_types::address_type serverAddress = MakeAddress(std::move(t_address), t_port);
    Create(m_socket);
    MakeAddressReused(m_socket);

    Bind(m_socket, serverAddress);
    SetSocketNonblocking(m_socket);
    Listen(m_socket);

    m_socketsSet = epoll_create(1);
    EpollCtlAdd(m_socketsSet, m_socket, EPOLLIN | EPOLLOUT | EPOLLET);
}

AsyncEchoServer::~AsyncEchoServer()
{
    CLOSE_SOCKET(m_socket);
}

void AsyncEchoServer::Run()
{
    int newFileDescriptors;
    cross_types::socket_type clientSocket;

    while (true)
    {
        try
        {
            newFileDescriptors = epoll_wait(m_socketsSet, m_events, MAX_EVENTS, -1);

            for (int i = 0; i < newFileDescriptors; ++i)
            {
                if (m_events[i].data.fd == m_socket)
                {
                    Accept(clientSocket, m_socket);
                    SetSocketNonblocking(clientSocket);
                    EpollCtlAdd(m_socketsSet, clientSocket,  EPOLLIN | EPOLLET | EPOLLRDHUP | EPOLLHUP);
                }
                else if (m_events[i].events & EPOLLIN)
                {

                    HandleClientConnection(m_events[i].data.fd);
                }

                if (m_events[i].events & (EPOLLRDHUP | EPOLLHUP))
                {
                    epoll_ctl(m_socketsSet,  EPOLL_CTL_DEL, m_events[i].data.fd, nullptr);
                    CLOSE_SOCKET(m_events[i].data.fd);
                    continue;
                }
            }
        }

        catch (...)
        {
            std::cout << "Catch an error!" << std::endl;
            CLOSE_SOCKET(clientSocket);
            break;
        }
    }
}
