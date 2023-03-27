#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <sys/socket.h>
#include <sstream>
#include <map>

namespace
{
    constexpr uint8_t SOCKET_ERROR = -1;
}

void Check(uint8_t expression, const std::string &message)
{
    if (expression == SOCKET_ERROR)
    {
        std::cerr << message << std::endl;
        exit(-1);
    }
}
#endif //UTILS_H
