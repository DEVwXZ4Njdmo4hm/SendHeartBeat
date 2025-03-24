#include "Server.h"

using namespace HeartBeat;

void HeartBeat::WriteToDB(const DataPkt &data)
{
    sqlite3 *db_raw = nullptr;
    int rc = sqlite3_open_v2(
        db_path.c_str(), &db_raw,
        SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE,
        nullptr);

    const std::unique_ptr<sqlite3, decltype([](sqlite3 *db) { sqlite3_close_v2(db); } )> db(db_raw);

    if (rc != SQLITE_OK)
    {
        std::println(stderr, "Can't open database: {}", sqlite3_errmsg(db.get()));
        return;
    }
    const auto createTableSQL = R"(
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
        boardModel, boardSerial, computerModel, computerSerial, computerSKU, computerUUID, computerFirmwareVersion, computerFirmwareManufacturer,
        load_cpuTotal, temp_coreMax, temp_coreAvg, temp_cpuPackage, volt_cpuCore, pwr_cpuPackage, pwr_cpuCore,
        load_gpuCore, clk_gpuCore, temp_gpuCore, pwr_gpuPackage
    ) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);
    )";

    char* errMsg = nullptr;
    rc = sqlite3_exec(db.get(), createTableSQL, nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK)
    {
        std::println(stderr, "SQL error: {}", errMsg);
        sqlite3_free(errMsg);
        return;
    }

    rc = sqlite3_exec(db.get(), "BEGIN TRANSACTION", nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK)
    {
        std::println(stderr, "SQL error: {}", errMsg);
        sqlite3_free(errMsg);
        return;
    }

    sqlite3_stmt *stmt_raw = nullptr;
    rc = sqlite3_prepare_v2(db.get(), insertSQL, -1, &stmt_raw, nullptr);
    const std::unique_ptr<sqlite3_stmt, decltype([](sqlite3_stmt *stmt) { sqlite3_finalize(stmt); } )> stmt(stmt_raw);
    if (rc != SQLITE_OK)
    {
        std::println(stderr, "SQL error: {}", sqlite3_errmsg(db.get()));
        return;
    }

    sqlite3_bind_int64(stmt.get(), 1, data.timestamp);

    // Bind machine info
    sqlite3_bind_text(stmt.get(), 2, UCharArrayToString(data.machInfo.boardModel).c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt.get(), 3, UCharArrayToString(data.machInfo.boardSerial).c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt.get(), 4, UCharArrayToString(data.machInfo.computerModel).c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt.get(), 5, UCharArrayToString(data.machInfo.computerSerial).c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt.get(), 6, UCharArrayToString(data.machInfo.computerSKU).c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt.get(), 7, UCharArrayToString(data.machInfo.computerUUID).c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt.get(), 8, UCharArrayToString(data.machInfo.computerFirmwareVersion).c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt.get(), 9, UCharArrayToString(data.machInfo.computerFirmwareManufacturer).c_str(), -1, SQLITE_TRANSIENT);

    // Bind CPU Sensor data
    sqlite3_bind_double(stmt.get(), 10, data.cpuInfo.load_cpuTotal);
    sqlite3_bind_double(stmt.get(), 11, data.cpuInfo.temp_coreMax);
    sqlite3_bind_double(stmt.get(), 12, data.cpuInfo.temp_coreAvg);
    sqlite3_bind_double(stmt.get(), 13, data.cpuInfo.temp_cpuPackage);
    sqlite3_bind_double(stmt.get(), 14, data.cpuInfo.volt_cpuCore);
    sqlite3_bind_double(stmt.get(), 15, data.cpuInfo.pwr_cpuPackage);
    sqlite3_bind_double(stmt.get(), 16, data.cpuInfo.pwr_cpuCore);

    // Bind GPU Sensor data
    sqlite3_bind_double(stmt.get(), 17, data.gpuInfo.load_gpuCore);
    sqlite3_bind_double(stmt.get(), 18, data.gpuInfo.clk_gpuCore);
    sqlite3_bind_double(stmt.get(), 19, data.gpuInfo.temp_gpuCore);
    sqlite3_bind_double(stmt.get(), 20, data.gpuInfo.pwr_gpuPackage);

    rc = sqlite3_step(stmt.get());
    if (rc != SQLITE_DONE)
    {
        std::println(stderr, "Failed to insert data: {}", sqlite3_errmsg(db.get()));
        sqlite3_exec(db.get(), "ROLLBACK", nullptr, nullptr, &errMsg);
        return;
    }

    rc = sqlite3_exec(db.get(), "COMMIT", nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK)
    {
        std::println(stderr, "SQL failed to commit transaction: {}", errMsg);
        sqlite3_free(errMsg);
        sqlite3_exec(db.get(), "ROLLBACK", nullptr, nullptr, &errMsg);
    }
}
