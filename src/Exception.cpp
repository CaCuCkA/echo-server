// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#include "Exception.h"

#include <sstream>

Exception::Exception(int t_code, int t_line, const char *t_file,const char *t_msg) noexcept
        :
        m_code(t_code), m_line(t_line), m_file(t_file), m_msg(t_msg)
{}

int Exception::GetErrorCode() const noexcept
{
    return m_code;
}

const char* Exception::what() const noexcept
{
    std::ostringstream oss;
    oss << GetOriginString();
    m_whatBuffer = oss.str();
    return m_whatBuffer.c_str();
}

const char* Exception::GetMessage() const noexcept
{
    return m_msg.c_str();
}

std::string Exception::GetOriginString() const noexcept
{
    std::ostringstream oss;
    oss << "[Code] " << m_code << std::endl
        << "[File] " << m_file << std::endl
        << "[Line] " << m_line << std::endl
        << "[Desc] " << m_msg;
    return oss.str();
}