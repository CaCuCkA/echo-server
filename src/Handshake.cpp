#include <openssl/sha.h>
#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/transform_width.hpp>

#include "Handshake.h"
#include "Exception.h"
#include "Server.h"

const std::string WEBSOCKET_GUID = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";

std::string Handshake::GetHeaderValue(const http::fields& headers, const std::string& header_key)
{
    auto it = headers.find(header_key);
    if (it != headers.end()) {
        return it->value().to_string();
    }
    return "";
}

void Handshake::Base64Encode(const std::string& input)
{
    using namespace boost::archive::iterators;

    using Base64EncodeIterator =
            base64_from_binary<transform_width<const char*, 6, 8>>;

    std::string encoded(
            Base64EncodeIterator(input.data()),
            Base64EncodeIterator(input.data() + SHA_DIGEST_LENGTH)
    );

    size_t padding = (3 - input.size() % 3) % 3;
    encoded.append(padding, '=');

    m_b64_string = std::move(encoded);
}

void Handshake::PerformWebsocketHandshake(const http::fields& headers, char* buffer)
{
    std::string sec_websocket_key = GetHeaderValue(headers, "Sec-WebSocket-Key");

    if (sec_websocket_key.empty())
    {
        throw EXCEPTION(EC_CANT_DO_HANDSHAKE, "Failed TCP handshake");
    }

    std::string concatenated_val = sec_websocket_key + WEBSOCKET_GUID;

    unsigned char sha1_hash[SHA_DIGEST_LENGTH];
    SHA1(reinterpret_cast<const unsigned char*>(concatenated_val.data()), concatenated_val.size(), sha1_hash);

    Base64Encode(reinterpret_cast<const char*>(sha1_hash));

    snprintf(buffer, BUFFER_SIZE,
             "HTTP/1.1 101 Switching Protocols\r\n"
             "Upgrade: websocket\r\n"
             "Connection: Upgrade\r\n"
             "Sec-WebSocket-Accept: %s\r\n\r\n",
             m_b64_string.substr(0, m_b64_string.size() - 1).c_str());
}
