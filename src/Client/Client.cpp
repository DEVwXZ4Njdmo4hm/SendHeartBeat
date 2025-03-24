#include "Client.h"

using namespace HeartBeat;

int main(int argc, char* argv[])// Initialize the COM library
{
    if (argc != 2)
    {
        std::println(stderr, "Usage: {} <server>", argv[0]);
        return 1;
    }

    std::string server = argv[1];

    if (server.empty() or not IsValidIPv4(server))
    {
        std::println(stderr, "Invalid server.");
        return 2;
    }

    std::thread DataHandler_thr(DataHandler, server, listen_port);

    std::println(stdout, "HeartBeat Client started.");
    std::println(stdout, "Press any key to exit.");
    std::cin.get();

    worker_signal = false;

    if (DataHandler_thr.joinable())
    {
        DataHandler_thr.join();
    }

    return 0;
}
