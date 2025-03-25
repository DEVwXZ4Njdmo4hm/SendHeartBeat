#pragma once

#include <print>
#include <mutex>
#include <thread>
#include <deque>
#include <queue>
#include <iostream>
#include <asio.hpp>
#include <sqlite3.h>

#include <DataPkt.h>
#include <Utils.h>

#include <config.h>

namespace HeartBeat {
    inline const auto createTableSQL = R"(
        CREATE TABLE IF NOT EXISTS Record (
            timestamp INTEGER PRIMARY KEY,
            boardModel TEXT,
            boardSerial TEXT,
            computerModel TEXT,
            computerSerial TEXT,
            computerSKU TEXT,
            computerUUID TEXT,
            computerFirmwareVersion TEXT,
            computerFirmwareManufacturer TEXT,
            load_cpuTotal REAL,
            temp_coreMax REAL,
            temp_coreAvg REAL,
            temp_cpuPackage REAL,
            volt_cpuCore REAL,
            pwr_cpuPackage REAL,
            pwr_cpuCore REAL,
            load_gpuCore REAL,
            clk_gpuCore REAL,
            temp_gpuCore REAL,
            pwr_gpuPackage REAL
        );
    )";

    const auto insertSQL = R"(
    INSERT INTO Record (
        timestamp,
        boardModel, boardSerial,
        computerModel, computerSerial, computerSKU, computerUUID,
        computerFirmwareVersion, computerFirmwareManufacturer,
        load_cpuTotal, temp_coreMax, temp_coreAvg, temp_cpuPackage, volt_cpuCore, pwr_cpuPackage, pwr_cpuCore,
        load_gpuCore, clk_gpuCore, temp_gpuCore, pwr_gpuPackage
    ) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);
    )";

    inline sqlite3* db = nullptr;

    inline std::mutex msg_queue_mutex;
    inline std::deque<serialized_data_t<DataPkt>> msg_queue;
    inline std::queue<serialized_data_t<DataPkt>> wait_queue;
    inline std::atomic_bool work_signal { true };
    inline std::atomic_bool watcher_halt { false };

    bool OpenDB();

    bool CloseDB();

    void Watcher();

    void DataHandler();

    bool WriteToDB(const DataPkt& data);
}