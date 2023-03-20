#include "EchoWebSocket.h"
#include <iostream>
#include <sys/syscall.h>
#ifndef SYS_gettid
#error "SYS_gettid unvailable on this system"
#endif

#define gettid() ((pid_t)syscall(SYS_gettid))


EchoWebSocket::EchoWebSocket(tcp::socket &&socket)
        :
        m_websocket(std::move(socket)) {}


void EchoWebSocket::Run()
{
    m_websocket.async_accept([self{shared_from_this()}](beast::error_code ec) {
        if (ec)
        {
            std::cout << ec.message() << std::endl;
            return;
        }
        self->Echo();
    });
}


void EchoWebSocket::Echo()
{
    m_websocket.async_read(m_buffer, [self{shared_from_this()}](beast::error_code ec,
                                                                std::size_t bytes_transferred)
    {
//        std::cout << "thread id: " << gettid() << std::endl;

         if (ec == websocket::error::closed)
        {
            return;
        }
        if (ec)
        {
            std::cout << ec.message() << std::endl;
            return;
        }
        auto output = beast::buffers_to_string(self->m_buffer.cdata());
        std::cout << output << std::endl;

        self->m_websocket.async_write(self->m_buffer.data(),[self](beast::error_code ec,
                std::size_t bytes_transferred )
          {
                if (ec)
                {
                    std::cout << ec.message() << std::endl;
                    return;
                }
                self->m_buffer.consume(self->m_buffer.size());
          });

        self->Echo();
    });
}


