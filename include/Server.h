#ifndef ECHO_SERVER_SERVER_H
#define ECHO_SERVER_SERVER_H

#include "Websocket.h"

#include <string>
#include <functional>


class Server
{
public:
    Server() = delete;
    Server(const Server&) = delete;
    Server& operator=(const Server&) = delete;
    Server(Server&&) = delete;
    Server& operator=(Server&&) = delete;

    Server(const std::string& t_address, unsigned char t_port, const std::function<void()> t_function);
    ~Server();

    void Run();
private:
    Websocket m_websocket;
    std::function<void()> m_function;
};

#endif //ECHO_SERVER_SERVER_H
