#include "Client.h"

using namespace HeartBeat;

void HeartBeat::Sender(std::string server, port_t port, serialized_data_t<DataPkt> &data)
{
    try {
        asio::io_context ioContext;
        asio::ip::tcp::socket socket(ioContext);

        asio::ip::tcp::endpoint endpoint(asio::ip::make_address(server), port);

        socket.connect(endpoint);

        asio::write(socket, asio::buffer(data, data.size()));

        socket.shutdown(asio::ip::tcp::socket::shutdown_both);
        socket.close();

        std::println(stdout, "Sender: Data sent successfully to {}:{}", server, port);
    }
    catch (const std::exception &e) {
        std::println(stderr, "Sender error: {}", e.what());
    }
}
