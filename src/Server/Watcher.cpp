#include <thread>

#include "Server.h"

using namespace HeartBeat;

void HeartBeat::Watcher(const port_t port) {
    try {
        asio::io_context ioContext;
        asio::executor_work_guard<asio::io_context::executor_type> workGuard =
            asio::make_work_guard(ioContext);

        asio::ip::tcp::endpoint endpoint(asio::ip::make_address("0.0.0.0"), port);
        asio::ip::tcp::acceptor acceptor(ioContext, endpoint);
        acceptor.non_blocking(true);
        acceptor.set_option(asio::ip::tcp::acceptor::reuse_address(true));

        std::thread ioThread([&ioContext]() { ioContext.run(); });

        while (work_signal)
        {
            if (watcher_halt)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                continue;
            }

            asio::ip::tcp::socket socket(ioContext);
            asio::error_code ec;

            acceptor.accept(socket, ec);
            if (ec == asio::error::would_block)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
                continue;
            }

            if (ec)
            {
                std::println(stderr, "Watcher accept error: {}", ec.message());
                continue;
            }

            std::println(stdout, "Watcher: New connection accepted, from {}", socket.remote_endpoint().address().to_string());

            serialized_data_t<DataPkt> buffer;
            auto bytesRead = asio::read(socket, asio::buffer(buffer), ec);

            if (ec)
            {
                std::println(stderr, "Watcher: Read error: {}", ec.message());
                socket.close();
                continue;
            }

            // Push data into the queue
            {
                std::lock_guard lock(msg_queue_mutex);
                msg_queue.push_back(buffer);
            }

            socket.close();
        }

        workGuard.reset();
        ioContext.stop();
        if (ioThread.joinable())
            ioThread.join();

        std::println(stdout, "Watcher: Stopped.");
    }
    catch (std::exception &e) {
        std::println(stdout, "Watcher exception: {}", e.what());
    }
}
