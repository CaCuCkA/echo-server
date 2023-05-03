#include "Utils.h"
#include "Exception.h"

#ifdef MULTI
#include "MultiEchoServer.h"
#else
#include "SyncEchoServer.h"
#endif

int main(int argc, char *argv[])
{
    if (IsEnoughArguments(argc))
    {
        throw EXCEPTION(EC_NOT_ENOUGH_ARGS, "Not enough arguments");
    }

#ifdef MULTI
    auto [address, port, amountOfThreads] = ParseValues(argv);
    MultiEchoServer::Get(std::move(address), port, amountOfThreads).Run();
#else
    auto [address, port] = ParseValues(argv);
    SyncEchoServer::Get(std::move(address), port).Run();
#endif

    return 0;
}