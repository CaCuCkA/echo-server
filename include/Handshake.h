#ifndef ECHO_SERVER_HANDSHAKE_H
#define ECHO_SERVER_HANDSHAKE_H

#include <iostream>
#include <boost/beast/http.hpp>

#include "CrossType.h"

namespace http = boost::beast::http;

class Handshake
{
public:
    Handshake() = default;
    ~Handshake() = default;
    Handshake(const Handshake&) = delete;
    Handshake& operator=(const Handshake&) = delete;
    Handshake(Handshake&&) = delete;
    Handshake& operator=(Handshake&&) = delete;

    void PerformWebsocketHandshake(const http::fields& headers, char* buffer);

private:
    void Base64Encode(const std::string& in);
    static std::string GetHeaderValue(const http::fields& headers, const std::string& header_key);
private:
    std::string m_b64_string;
};

#endif //ECHO_SERVER_HANDSHAKE_H
