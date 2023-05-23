#include "Utils.h"
#include "Exception.h"
#include "Handshake.h"
#ifdef MULTI
#include "MultiEchoServer.h"
#elif SYNC
#include "SyncEchoServer.h"
#else
#include "AsyncEchoServer.h"
#endif

int main(int argc, char *argv[])
{
    if (IsEnoughArguments(argc))
    {
        throw EXCEPTION(EC_NOT_ENOUGH_ARGS, "Not enough arguments");
    }

#ifdef MULTI
    auto [address, port, amountOfThreads] = ParseValues(argv);
    MultiEchoServer server(std::move(address), port, amountOfThreads);
#elif SYNC
    auto [address, port] = ParseValues(argv);
    SyncEchoServer server(std::move(address), port);
#else
    auto [address, port] = ParseValues(argv);
    AsyncEchoServer server(std::move(address), port);
#endif

    server.Run();


    return 0;
}