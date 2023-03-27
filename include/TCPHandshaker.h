#ifndef GENERATE_RESPONSE_H
#define GENERATE_RESPONSE_H

#include <map>
#include <string>

class TCPHandshaker
{
public:
    TCPHandshaker() = delete;
    ~TCPHandshaker() = default;
    TCPHandshaker(const TCPHandshaker&) = delete;
    TCPHandshaker& operator=(const TCPHandshaker&) = delete;
    TCPHandshaker(TCPHandshaker&&) = delete;
    TCPHandshaker& operator=(TCPHandshaker&&) = delete;

    TCPHandshaker(int t_length, char* t_buffer);

    int PerformWebsocketHandshake(int clientSocket);

private:
    static std::string Sha1(const std::string& input);
    static std::string Base64Encode(const std::string& input);
    static std::string ConstructWebsocketUpgradeResponse(const std::string& acceptKey);

    void ParseHTTPRequest();
    bool IsWebsocketUpgradeRequest();
    std::string GenerateAcceptKey();

private:
    std::string m_request;
    std::map<std::string, std::string> m_headers;
    static const std::string WEBSOCKET_MAGIC_STRING;
};
#endif //GENERATE_RESPONSE_H
