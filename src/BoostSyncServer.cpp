#include <iostream>
#include <thread>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>

namespace beast = boost::beast;
namespace http = beast::http;
namespace websocket = beast::websocket;
namespace net = boost::asio;
using tcp = boost::asio::ip::tcp;

int main(int args, char* argv[])
{
    const auto address = net::ip::make_address("127.0.0.1");
    const auto port = static_cast<unsigned short>(std::atoi("8083"));

    net::io_context ioc{1};

    tcp::acceptor acceptor{ioc, {address, port}};

    while (true)
    {
        tcp::socket socket{ioc};
        acceptor.accept(socket);
        std::cout << "Connection accepted" << std::endl;

        std::thread
        {
            [q {std::move(socket)}]()
            {
                websocket::stream<tcp::socket> ws {std::move(const_cast<tcp::socket&>(q))};
                ws.accept();

                while(true)
                {
                    try
                    {
                        beast::flat_buffer buffer;
                        ws.read(buffer);
                        auto out = beast::buffers_to_string(buffer.cdata());
                        std::cout << out << std::endl;

                        ws.write(buffer.data());
                    }
                    catch (const beast::system_error& se)
                    {
                        if (se.code() != websocket::error::closed)
                        {
                            std::cerr << se.what() << std::endl;
                            break;
                        }
                    }
                }
            }
        }.detach();
    }
    return 0;
}
