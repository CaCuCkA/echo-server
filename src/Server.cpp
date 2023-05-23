#include <cstring>
#include <iostream>

#include "Server.h"
#include "CrossType.h"
#include "Exception.h"
#include "Handshake.h"

#define MAX_FRAME_SIZE (2^32)

void Server::MakeAddressReused(socket_type &t_socket)
{
    int optionValue = 1;
    SetSocketOptions(t_socket, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<char *>(&optionValue), sizeof(int));
}

void Server::CleanBuffer(char *buffer)
{
    memset(buffer, 0, BUFFER_SIZE);
}

http::fields Server::ParseHeaders(const char *header)
{
    http::fields fields;

    http::request_parser<http::empty_body> parser;
    boost::beast::error_code ec;

    std::string HEADER(header);

    parser.put(boost::asio::buffer(HEADER), ec);
    if (!ec)
    {
        fields = parser.get().base();
    }
    return fields;
}

void Server::ConnectionWithHandshake(cross_types::socket_type& t_socket, const http::fields& fields)
{
    char buffer[BUFFER_SIZE];
    unsigned char smallPayloadLen;
    unsigned char maskOffset;
    unsigned long payloadLen;

    Handshake handshake;
    handshake.PerformWebsocketHandshake(fields, buffer);
    auto bytesRecv = static_cast<cross_types::recv_type>(strlen(buffer));
    Send(t_socket, buffer, bytesRecv);

    while (true)
    {
        CleanBuffer(buffer);

        try
        {
            bytesRecv = Read(t_socket, buffer, BUFFER_SIZE);
            if (!bytesRecv)
            {
                break;
            }

            smallPayloadLen = buffer[1] & 0x7F;

            if (smallPayloadLen < 126)
            {
                payloadLen = smallPayloadLen;
                maskOffset = 2;
            }
            else if (smallPayloadLen == 126)
            {
                unsigned short payloadLenNbo = *(reinterpret_cast<unsigned short *>(buffer) + 2);
                payloadLen = ntohs(payloadLenNbo);
                maskOffset = 4;
            }
            else
            {
                break;
            }
            unsigned char maskingKey[4];
            maskingKey[0] = buffer[maskOffset];
            maskingKey[1] = buffer[maskOffset + 1];
            maskingKey[2] = buffer[maskOffset + 2];
            maskingKey[3] = buffer[maskOffset + 3];

            std::string maskedPayloadData(static_cast<char*>(buffer) + maskOffset + 4, payloadLen);

            for (size_t i = 0; i < payloadLen; ++i)
            {
                char mask = maskingKey[i % 4];
                maskedPayloadData[i] = maskedPayloadData[i] ^ mask;
            }

            SendWebsocketFrame(t_socket, maskedPayloadData, static_cast<cross_types::recv_type>(payloadLen));
        }
        catch(...)
        {
            break;
        }
    }
    CLOSE_SOCKET(t_socket);
}

void Server::HandleClientConnection(int t_socket)
{
    char buffer[BUFFER_SIZE];
    cross_types::recv_type bytesRecv;

    while (true)
    {
        CleanBuffer(buffer);
        try
        {
            bytesRecv = Read(t_socket, buffer, BUFFER_SIZE);

            if (!bytesRecv)
            {
                CLOSE_SOCKET(t_socket);
                break;
            }

            const http::fields& fields = ParseHeaders(buffer);

            if (fields.begin() != fields.end())
            {
                ConnectionWithHandshake(t_socket, fields);
                return;
            }

            Send(t_socket, buffer, bytesRecv);
        }
        catch (...)
        {
            CLOSE_SOCKET(t_socket);
            break;
        }
    }
}

cross_types::address_type Server::MakeAddress(std::string &&t_address, uint16_t t_port)
{
    cross_types::address_type address{};
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    inet_pton(AF_INET, t_address.c_str(), &address.sin_addr);
    address.sin_port = htons(t_port);
    return address;
}

void Server::SetSocketNonblocking(cross_types::socket_type &t_socket)
{
    Fcntl(t_socket, F_SETFD, Fcntl(t_socket, F_GETFD, 0) | O_NONBLOCK);
}

void Server::Create(cross_types::socket_type &t_socket)
{
    #if defined(_WIN32) || defined(WIN32)
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        {
            WSACleanup();
            throw EXCEPTION(EC_CANT_CREATE_SOCKET, "Can`t create socket");
        }
    #endif

    t_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (t_socket == EC_CANT_CREATE_SOCKET)
    {
        CLOSE_SOCKET(t_socket);
        throw EXCEPTION(EC_CANT_CREATE_SOCKET, "Can`t create socket");
    }
}

void Server::Bind(cross_types::socket_type &t_socket, cross_types::address_type &t_address)
{
    auto error = bind(t_socket,
                      reinterpret_cast<cross_types::socket_address *>(&t_address),
                      sizeof(t_address));
    if (error == EC_CANT_BIND_SOCKET)
    {
        CLOSE_SOCKET(t_socket);
        throw EXCEPTION(EC_CANT_BIND_SOCKET, "Can`t bind socket");
    }
}

void Server::Listen(cross_types::socket_type &t_socket)
{
    auto error = listen(t_socket, SOMAXCONN);
    if (error == EC_CANT_LISTEN_SOCKET)
    {
        CLOSE_SOCKET(t_socket);
        throw EXCEPTION(EC_CANT_LISTEN_SOCKET, "Failed to listen socket");
    }
}

void Server::EpollCtlAdd(int epollFileDesc, int t_socket, uint32_t events)
{
    epoll_event epollEvents{};
    epollEvents.events = events;
    epollEvents.data.fd = t_socket;
    if (epoll_ctl(epollFileDesc, EPOLL_CTL_ADD, t_socket, &epollEvents) == EC_CANT_ADD_SOCKET_TO_EPOLL)
    {
        throw EXCEPTION(EC_CANT_ADD_SOCKET_TO_EPOLL, "Failed to add socket to epoll");
    }
}

void Server::SetSocketOptions(cross_types::socket_type &t_socket, int level, int socketOption, cross_types::option_type buffer,
                              size_t bufferSize)
{
    auto error = setsockopt(t_socket, level, socketOption, buffer, bufferSize);
    if (error == EC_CANT_SET_SOCKET_OPTION)
    {
        CLOSE_SOCKET(t_socket);
        throw EXCEPTION(EC_CANT_SET_SOCKET_OPTION, "Failed to set socket option");
    }
}

void Server::Accept(cross_types::socket_type &t_socket, cross_types::socket_type &listeningSocket,
                    cross_types::socket_address *socketAddress, cross_types::address_len *addressLength)
{
    t_socket = accept(listeningSocket, socketAddress, addressLength);
    if (t_socket == EC_CANT_ACCEPT_SOCKET)
    {
        throw EXCEPTION(EC_CANT_LISTEN_SOCKET, "Failed to listen socket");
    }
}

void Server::Accept(cross_types::socket_type& t_socket, cross_types::socket_type& listeningSocket)
{
    Accept(t_socket, listeningSocket, nullptr, nullptr);
}

cross_types::recv_type Server::Read(cross_types::socket_type &t_socket, char *buffer, uint16_t bufferLength)
{
    cross_types::recv_type bytesRecv = recv(t_socket, buffer, bufferLength, 0);
    if (bytesRecv == EC_CANT_READ_DATA)
    {
        CLOSE_SOCKET(t_socket);
        throw EXCEPTION(EC_CANT_READ_DATA, "Failed to read data from socket");
    }
    return bytesRecv;
}

cross_types::send_type
Server::Send(cross_types::socket_type &t_socket, char *buffer, cross_types::recv_type bytesRecv, int flag)
{
    auto sendBytes = send(t_socket, buffer, bytesRecv, flag);
    if (sendBytes == EC_CANT_SEND_DATA)
    {
        CLOSE_SOCKET(t_socket);
        throw EXCEPTION(EC_CANT_SEND_DATA, "Failed to send data from socket");
    }
    else if (sendBytes < bytesRecv)
    {
        Send(t_socket, buffer + sendBytes, bytesRecv - sendBytes, flag);
    }
    return sendBytes;
}

void Server::SendWebsocketFrame(cross_types::socket_type &t_socket, std::string &payload, size_t payloadLength)
{
    unsigned char frameBuffer[MAX_FRAME_SIZE];

    frameBuffer[0] = 0x81;

    long offset;

    if (payloadLength < 126)
    {
        offset = 2;
        frameBuffer[1] = (char) payloadLength;
    }
    else if (payloadLength < 65536)
    {
        offset = 4;
        frameBuffer[1] = 126;
        *((short *) frameBuffer + 2) = static_cast<short>(htons(payloadLength));
    }
    else
    {
        throw EXCEPTION(EC_CANT_SEND_FRAME, "Failed to send frame");
    }

    memcpy(frameBuffer + offset, payload.c_str(), payloadLength);

    cross_types::recv_type messageSize = offset + payloadLength;

    Send(t_socket, reinterpret_cast<char *>(frameBuffer), messageSize);
}
