#include "Client.h"

using namespace HeartBeat;

int main(int argc, char* argv[])// Initialize the COM library
{
    if (argc != 3)
    {
        std::println(stderr, "Usage: {} <server> <port>", argv[0]);
        return 1;
    }

    std::string server = argv[1];
    port_t port = static_cast<port_t>(std::stoi(argv[2]));

    if (server.empty() or not IsValidIPv4(server))
    {
        std::println(stderr, "Invalid server.");
        return 2;
    }

    if (not port)
    {
        std::println(stderr, "Invalid port.");
        return 3;
    }

    std::thread DataHandler_thr(DataHandler, server, port);

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
