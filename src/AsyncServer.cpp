#include "Listener.h"

#include <boost/beast/core.hpp>
#include <memory>
#include <vector>
#include <thread>


namespace beast = boost::beast;
namespace http = beast::http;
namespace websocket = beast::websocket;
namespace net = boost::asio;
using tcp = boost::asio::ip::tcp;


int main(int args, char* argv[])
{
    const uint16_t port = 8083;
    const uint8_t threads=std::thread::hardware_concurrency();
    net::io_context ioc{threads};
    std::make_shared<Listener>(ioc, port)->AsyncAccept();

    std::vector<std::thread> v;
    v.reserve(threads - 1);

    for (uint8_t i = threads - 1; i > 0; --i)
    {
        v.emplace_back([&ioc]()
       {
            ioc.run();
       });
    }

    ioc.run();
}
