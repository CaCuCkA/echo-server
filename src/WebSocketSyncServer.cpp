#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string>
#include <cstring>


int main(int args, char* argv[])
{
    int listening = socket(AF_INET, SOCK_STREAM, 0);

    if (listening == -1)
    {
        std::cerr << "Can`t create a socket!" << std::endl;
        return -1;
    }

    sockaddr_in hint{};
    hint.sin_family = AF_INET;
    hint.sin_port = htons(54000);
    inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);

    if (bind(listening, reinterpret_cast<const sockaddr *>(&hint), sizeof(hint)) == -1)
    {
        std::cerr << "Can`t bind to IP/port" << std::endl;
        return -2;
    }

    if (listen(listening, SOMAXCONN) == -1)
    {
        std::cerr << "Can`t listen" << std::endl;
        return -3;
    }

    sockaddr_in client{};
    socklen_t clientSize = sizeof(client);
    char host[NI_MAXHOST];
    char service[NI_MAXSERV];

    int clientSocket = accept(listening,
                              reinterpret_cast<sockaddr *>(&client),
                              &clientSize);

    if (clientSocket == -1)
    {
        std::cerr << "Problem with client connecting" << std::endl;
        return -4;
    }

    close(listening);

    memset(host, 0, NI_MAXHOST);
    memset(service, 0, NI_MAXSERV);

    int result = getnameinfo(reinterpret_cast<sockaddr *>(&client),
                             sizeof(client),
                             host,
                             NI_MAXHOST,
                             service,
                             NI_MAXSERV,
                             0);

    if (result)
    {
        std::cout << host << " connected on service " << service << std::endl;
    }
    else
    {
        inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
        std::cout << host << " connected on " << ntohs(client.sin_port) << std::endl;
    }

    char buffer[4096];

    while (true)
    {
        memset(buffer, 0, 4096);

        int bytesReceive = recv(clientSocket, buffer, 4096, 0);
        if (bytesReceive == -1)
        {
            std::cerr << "There was a connect issue" << std::endl;
            break;
        }

        if (bytesReceive == 0)
        {
            std::cout << "The client disconnected" << std::endl;
            break;
        }

        std::cout << "Received: " << std::string(buffer, 0, bytesReceive) << std::endl;

        send(clientSocket, buffer, bytesReceive + 1, 0);
    }

    close(clientSocket);

    return 0;
}