#ifndef ECHO_WEBSOCKET_H
#define ECHO_WEBSOCKET_H

#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>

namespace beast = boost::beast;
namespace websocket = beast::websocket;
using tcp = boost::asio::ip::tcp;

class EchoWebSocket : public std::enable_shared_from_this<EchoWebSocket>
{
public:
    EchoWebSocket() = delete;
    ~EchoWebSocket() = default;
    EchoWebSocket(const EchoWebSocket&) = delete;
    EchoWebSocket& operator=(const EchoWebSocket&) = delete;
    EchoWebSocket(EchoWebSocket&&) = delete;
    EchoWebSocket& operator=(EchoWebSocket&&) = delete;

    EchoWebSocket(tcp::socket&& socket);

    void Run();

private:
    void Echo();

private:
    websocket::stream<beast::tcp_stream> m_websocket;
    beast::flat_buffer m_buffer;
};
#endif //ECHO_WEBSOCKET_H
