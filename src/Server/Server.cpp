#include "Server.h"

using namespace HeartBeat;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::println(stderr, "Usage: {} <port>", argv[0]);
        return 1;
    }

    auto port = static_cast<port_t>(std::stoi(argv[1]));
    if (not port)
    {
        std::println(stderr, "Invalid port number");
        return 2;
    }

    std::thread watcher_thr(Watcher, port);
    std::thread data_handler_thr(DataHandler);

    std::println(stdout, "Server started on port {}", port);
    std::println(stdout, "Press enter to stop the server...");
    std::cin.get();

    work_signal = false;
    if (watcher_thr.joinable())
        watcher_thr.join();

    if (data_handler_thr.joinable())
        data_handler_thr.join();

    std::println(stdout, "Server stopped");

    return 0;
}
