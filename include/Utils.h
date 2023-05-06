#ifndef ECHO_SERVER_UTILS_H
#define ECHO_SERVER_UTILS_H

#include <tuple>
#include <cstdint>
#include <string>


inline bool IsEnoughArguments(uint8_t argument)
{
    return argument <= 2;
}

#ifdef MULTI
inline std::tuple<std::string, uint16_t, uint16_t> ParseValues(char *argv[])
{
    return std::make_tuple(std::string(argv[1]), atoi(argv[2]), atoi(argv[3]));
}
#else
inline std::tuple<std::string, uint16_t> ParseValues(char *argv[])
{
    return std::make_tuple(std::string(argv[1]), std::stoi(argv[2]));
};
#endif //MULTI

#endif //ECHO_SERVER_UTILS_H
