#pragma once

#include <print>
#include <mutex>
#include <thread>
#include <deque>
#include <iostream>
#include <asio.hpp>
#include <sqlite3.h>

#include <DataPkt.h>
#include <Utils.h>

namespace HeartBeat {

    inline std::mutex msg_queue_mutex;
    inline std::deque<serialized_data_t<DataPkt>> msg_queue;
    inline std::atomic_bool work_signal { true };
    inline std::atomic_bool watcher_halt { false };

    void Watcher(port_t port);

    void DataHandler();

    void WriteToDB(DataPkt& data);
}