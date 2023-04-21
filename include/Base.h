#ifndef ECHO_SERVER_BASE_SOCKET_H
#define ECHO_SERVER_BASE_SOCKET_H

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

class Base
{
public:
    virtual ~Base() = default;

private:
    virtual cross_types::address_type MakeAddress() = 0;

    virtual void Create(cross_types::socket_type& socket) = 0;
    virtual void Bind(cross_types::socket_type& socket) = 0;
    virtual void Listen(cross_types::socket_type& socket) = 0;
    /*
    virtual void Accept(cross_types::socket_type& socket) = 0;
    virtual void Read(cross_types::socket_type& socket) = 0;
    virtual void Write(cross_types::socket_type& socket) = 0;
    */
};
#endif //ECHO_SERVER_BASE_SOCKET_H
