#include "Listener.h"
#include "EchoWebSocket.h"

#include <boost/asio/strand.hpp>
#include <iostream>


Listener::Listener(net::io_context &t_ioc, unsigned short t_port)
        :
        m_ioc(t_ioc), m_acceptor(net::make_strand(t_ioc), {net::ip::make_address("127.0.0.1"), t_port}) {}


void Listener::AsyncAccept()
{
    m_acceptor.async_accept(net::make_strand(m_ioc), [self{shared_from_this()}](boost::system::error_code ec,
            tcp::socket socket)
    {
        std::make_shared<EchoWebSocket>(std::move(socket))->Run();
        std::cout << "Connection accepted" << std::endl;
        self->AsyncAccept();
    });
}
