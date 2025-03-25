#include "Server.h"

using namespace HeartBeat;

int main()
{
    if (not OpenDB())
    {
        std::println(stderr, "Failed to open database");
        return 1;
    }

    std::thread watcher_thr(Watcher);
    std::thread data_handler_thr(DataHandler);

    std::println(stdout, "Server started on port {}", listen_port);
    std::println(stdout, "Press enter to stop the server...");
    std::cin.get();

    work_signal = false;
    if (watcher_thr.joinable())
        watcher_thr.join();

    if (data_handler_thr.joinable())
        data_handler_thr.join();


    CloseDB();
    std::println(stdout, "Server stopped");

    return 0;
}
