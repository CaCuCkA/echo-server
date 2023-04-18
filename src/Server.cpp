#include "Server.h"

Server::Server(const std::string &t_address, unsigned char t_port, const std::function<void()> t_function)
    :
    m_websocket(t_address, t_port), m_function(t_function)
{}

void Server::Run()
{
    m_websocket.Listen();
    m_function();
}