#ifndef ECHO_SERVER_UTILS_H
#define ECHO_SERVER_UTILS_H

#include <tuple>
#include <cstdint>
#include <string>

#ifdef MULTI
bool IsEnoughArguments(uint8_t argument)
{
    return argument <= 3;
}

std::tuple<std::string, uint16_t, uint16_t> ParseValues(char *argv[])
{
    return std::make_tuple(std::string(argv[1]), atoi(argv[2]), atoi(argv[3]));
};
#else
bool IsEnoughArguments(uint8_t argument)
{
    return argument <= 2;
}

std::tuple<std::string, uint16_t> ParseValues(char *argv[])
{
    return std::make_tuple(std::string(argv[1]), atoi(argv[2]));
};
#endif //MULTI

#endif //ECHO_SERVER_UTILS_H
