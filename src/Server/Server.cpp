#include "Server.h"

using namespace HeartBeat;

int main()
{
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

    std::println(stdout, "Server stopped");

    return 0;
}
