#include "Server.h"
#include "ThreadSafeQueue.h"
#include "CrossType.h"
#include "Exception.h"

#include <iostream>
#include <cstring>

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        throw EXCEPTION(EC_NOT_ENOUGH_ARGS, "Not enough arguments");
    }

    ThreadSafeQueue<cross_types::socket_type> refreshSocketQueue;
    ThreadSafeQueue<cross_types::socket_type> usedSocketQueue;

    auto functionToExecute = [&refreshSocketQueue, &usedSocketQueue](cross_types::socket_type serverSocket)
    {
        constexpr int maxClients = 4;

        for (int i = 0; i < maxClients; ++i)
        {
            cross_types::socket_type socket;
            usedSocketQueue.Enque(socket);

            std::thread clientThread{[&refreshSocketQueue, &usedSocketQueue]()
                                     {
                                         char buffer[1028];
                                         ssize_t bytesReceive;
                                         cross_types::socket_type clientSocket;
                                         while (true)
                                         {
                                             refreshSocketQueue.Deque(clientSocket);
                                             memset(buffer, 0, sizeof(buffer));
                                             bytesReceive = recv(clientSocket, buffer, sizeof(buffer), 0);
                                             if (bytesReceive < 0)
                                             {
                                                 std::cout << "FAILED TO READ FROM CLIENT SOCKET" << std::endl;
                                                 break;
                                             }
                                             std::cout << buffer << std::endl;
                                             send(clientSocket, buffer, bytesReceive, 0);
                                             CLOSE_SOCKET(clientSocket);
                                             usedSocketQueue.Enque(clientSocket);
                                         }

                                         std::cout << "SOCKET CLOSED AND THREAD DIED" << std::endl;
                                     }};

            clientThread.detach();
        }
        cross_types::socket_type clientSocket;
        while (true)
        {
            usedSocketQueue.Deque(clientSocket);
            clientSocket = accept(serverSocket, nullptr, nullptr);
            if (clientSocket == EC_CANT_ACCEPT_SOCKET) {
                std::cout << "Skip" << std::endl;
                continue;
            }

            refreshSocketQueue.Enque(clientSocket);
        }
    };

    Server server(argv[1], atoi(argv[2]), functionToExecute);
    server.Run();

    return 0;
}