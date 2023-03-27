#include "TCPHandshaker.h"

#include <sstream>
#include <openssl/sha.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <iomanip>
#include <sys/socket.h>

const std::string TCPHandshaker::WEBSOCKET_MAGIC_STRING = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";

TCPHandshaker::TCPHandshaker(int t_length, char *t_buffer)
        :
m_request(t_buffer, t_length)
{}


std::string TCPHandshaker::Sha1(const std::string &input)
{
    unsigned char hash[SHA_DIGEST_LENGTH];
    std::stringstream ss;
    SHA1((const unsigned char *) input.c_str(), input.length(), hash);

    for (unsigned char i: hash)
    {
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(i);
    }

    return ss.str();
}


std::string TCPHandshaker::Base64Encode(const std::string &input)
{
    BIO *b64 = BIO_new(BIO_f_base64());
    BIO *bioMemory = BIO_new(BIO_s_mem());
    BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
    BIO_push(b64, bioMemory);
    BIO_write(b64, input.c_str(), static_cast<int>(input.length()));
    BIO_flush(b64);
    char *outputBuffer = nullptr;
    size_t outputLength = BIO_get_mem_data(bioMemory, &outputBuffer);
    std::string output(outputBuffer, outputLength);
    BIO_free_all(b64);
    return output;
}


void TCPHandshaker::ParseHTTPRequest()
{
    std::stringstream ss(m_request);
    std::string method, path, version;
    ss >> method >> path >> version;
    std::string headerName, headerValue;

    while (std::getline(ss, headerName, ':') && std::getline(ss, headerValue, '\n'))
    {
        m_headers[headerName] = headerValue;
    }
}


bool TCPHandshaker::IsWebsocketUpgradeRequest()
{
    std::istringstream ss(m_request);
    std::string method;

    std::getline(ss, method, ' ');

    return (method == "GET" && m_request.find("Upgrade: websocket") != std::string::npos &&
            m_request.find("Sec-WebSocket-Key: ") != std::string::npos);
}


std::string TCPHandshaker::GenerateAcceptKey()
{
    std::string acceptKey = m_headers["Sec-WebSocket-Key"] + TCPHandshaker::WEBSOCKET_MAGIC_STRING;
    acceptKey = Sha1(acceptKey);
    acceptKey = Base64Encode(acceptKey);

    return acceptKey;
}


std::string TCPHandshaker::ConstructWebsocketUpgradeResponse(const std::string &acceptKey)
{
    std::stringstream responseSs;
    responseSs << "HTTP/1.1 101 Switching Protocols\r\n";
    responseSs << "Connection: Upgrade\r\n";
    responseSs << "Upgrade: websocket\r\n";
    responseSs << "Sec-WebSocket-Accept: " << acceptKey << "\r\n\r\n";

    return responseSs.str();
}


int TCPHandshaker::PerformWebsocketHandshake(int clientSocket)
{
    int result = -1;

    ParseHTTPRequest();

    if (IsWebsocketUpgradeRequest())
    {
        auto acceptKey = GenerateAcceptKey();
        auto response = ConstructWebsocketUpgradeResponse(acceptKey);

        result = send(clientSocket, response.c_str(), response.length(), 0);
    }

    return result;
}
