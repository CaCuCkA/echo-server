#include "MultiEchoServer.h"
#include "CrossType.h"

#include <thread>
#include <vector>
#include <cstring>

MultiEchoServer::MultiEchoServer(std::string &&t_address, uint16_t t_port, uint8_t t_amountOfThreads)
        :
        m_amountOfThreads(t_amountOfThreads)
{
    cross_types::address_type serverAddress = MakeAddress(std::move(t_address), t_port);
    Create(m_socket);
    MakeAddressReused(m_socket);
    Bind(m_socket, serverAddress);
    Listen(m_socket);
}

MultiEchoServer::~MultiEchoServer()
{
    CLOSE_SOCKET(m_socket);
}

void MultiEchoServer::Run()
{
    cross_types::queue_type<socket_type> usedSocketQueue;
    cross_types::queue_type<socket_type> refreshedSocketQueue;
    std::vector<std::thread> threads;

    socket_type clientSocket;

    for (uint8_t i = 0; i < m_amountOfThreads; ++i)
    {
        socket_type socket;
        usedSocketQueue.push(socket);
        std::thread clientThread {[&usedSocketQueue, &refreshedSocketQueue]()
                                  {
                                      socket_type clientSocket;

                                      while (true)
                                      {
                                          refreshedSocketQueue.pop(clientSocket);
                                          HandleClientConnection(clientSocket);
                                          usedSocketQueue.push(clientSocket);
                                      }
                                  }};

        clientThread.detach();
    }

    while (true)
    {
        usedSocketQueue.pop(clientSocket);
        try
        {
            Accept(clientSocket, m_socket);
        }
        catch(...)
        {
            continue;
        }
        refreshedSocketQueue.push(clientSocket);
    }
}
