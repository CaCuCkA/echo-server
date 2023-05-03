#include "MultiEchoServer.h"
#include "ThreadSafeQueue.h"

#include <thread>
#include <vector>
#include <cstring>

MultiEchoServer::MultiEchoServer(std::string &&t_address, uint16_t t_port, uint8_t t_amountOfThreads)
    :
m_amountOfThreads(t_amountOfThreads)
{
    cross_types::address_type serverAddress = MakeAddress(std::move(t_address), t_port);
    Create(m_socket);
    Bind(m_socket, serverAddress);
    Listen(m_socket);
}

MultiEchoServer::~MultiEchoServer() noexcept
{
    CLOSE_SOCKET(m_socket);
}

void MultiEchoServer::Run()
{
    ThreadSafeQueue<socket_type> usedSocketQueue;
    ThreadSafeQueue<socket_type> refreshedSocketQueue;
    std::vector<std::thread> threads;

    socket_type clientSocket;

    for (uint8_t i = 0; i < m_amountOfThreads; ++i)
    {
        socket_type socket;
        usedSocketQueue.Enque(socket);
        std::thread clientThread {[&usedSocketQueue, &refreshedSocketQueue, this]()
        {
            char buffer[BUFFER_SIZE];
            cross_types::recv_type bytesReceive;
            socket_type clientSocket;

            while (true)
            {
                refreshedSocketQueue.Deque(clientSocket);
                memset(buffer, 0, bufferSize);
                try
                {
                    bytesReceive = Read(clientSocket, buffer, BUFFER_SIZE);
                    Send(clientSocket, buffer, bytesReceive);
                }
                catch(...)
                {
                    break;
                }
                CLOSE_SOCKET(clientSocket);
                usedSocketQueue.Enque(clientSocket);
            }
        }};

        clientThread.detach();
    }

    while (true)
    {
        usedSocketQueue.Deque(clientSocket);
        try
        {
            Accept(clientSocket, m_socket);
        }
        catch(...)
        {
            continue;
        }
        refreshedSocketQueue.Enque(clientSocket);
    }
}