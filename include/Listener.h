#ifndef LISTENER_H
#define LISTENER_H

#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>

namespace net = boost::asio;
using tcp = boost::asio::ip::tcp;

class Listener : public std::enable_shared_from_this<Listener>
{
public:
    Listener() = delete;
    ~Listener() = default;
    Listener(const Listener&) = delete;
    Listener& operator=(const Listener&) = delete;
    Listener(Listener&&) = delete;
    Listener& operator=(Listener&&) = delete;

    Listener(net::io_context &t_ioc, unsigned short t_port);

    void AsyncAccept();

private:
    net::io_context& m_ioc;
    tcp::acceptor m_acceptor;
};
#endif //LISTENER_H
