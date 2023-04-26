#ifndef ECHO_SERVER_EXCEPTION_H
#define ECHO_SERVER_EXCEPTION_H

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "CrossType.h"
#include <exception>
#include <string>

#define EXCEPTION(code, msg) Exception(code, __LINE__, __FILE__, msg)

enum ErrorCodes
{
    EC_OK = 0,
    EC_NOT_ENOUGH_ARGS = 2,
#if defined(_WIN32) || defined(WIN32)
#define WIN_ERROR (-1)
    EC_CANT_CREATE_SOCKET = INVALID_SOCKET,
    EC_CANT_BIND_SOCKET = SOCKET_ERROR,
    EC_CANT_LISTEN_SOCKET = INVALID_SOCKET,
    EC_CANT_ACCEPT_SOCKET = INVALID_SOCKET,
    EC_CANT_READ_DATA = WIN_ERROR,
    EC_CANT_SEND_DATA = WIN_ERROR,
#else
#define POSIX_ERROR (-1)
    EC_CANT_CREATE_SOCKET = POSIX_ERROR,
    EC_CANT_BIND_SOCKET = POSIX_ERROR,
    EC_CANT_LISTEN_SOCKET = POSIX_ERROR,
    EC_CANT_ACCEPT_SOCKET = POSIX_ERROR,
    EC_CANT_READ_DATA = POSIX_ERROR,
    EC_CANT_SEND_DATA = POSIX_ERROR,
#endif
};

class Exception : public std::exception
{
public:
    Exception() = delete;
    Exception(const Exception&) = delete;
    Exception& operator=(const Exception&) = delete;
    Exception(Exception&&) = delete;
    Exception& operator=(Exception&&) = delete;

    Exception(int t_code, int t_line, const char* t_path, const char* t_msg) noexcept;
    ~Exception() override = default;

    const char* what() const noexcept override;
    const char* GetMessage() const noexcept;
    int GetErrorCode() const noexcept;
private:
    std::string GetOriginString() const noexcept;
private:
    int m_code;
    int m_line;
    std::string m_file;
    std::string m_msg;
    mutable std::string m_whatBuffer;
};
#endif //ECHO_SERVER_EXCEPTION_H
