#include "Server.h"

using namespace HeartBeat;

bool HeartBeat::OpenDB()
{
    int rc = sqlite3_open_v2(
        db_path.c_str(), &db,
        SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE,
        nullptr);

    if (rc != SQLITE_OK)
    {
        std::println(stderr, "Can't open database: {}", sqlite3_errmsg(db));
        return false;
    }

    char* errMsg = nullptr;
    rc = sqlite3_exec(db, createTableSQL, nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK)
    {
        std::println(stderr, "SQL error: {}", errMsg);
        sqlite3_free(errMsg);

        CloseDB();

        return false;
    }

    std::println(stdout, "Database opened successfully");
    return true;
}

bool HeartBeat::CloseDB()
{
    if (db)
    {
        sqlite3_close(db);
        db = nullptr;
    }

    return true;
}


bool HeartBeat::WriteToDB(const DataPkt &data)
{
    char* errMsg = nullptr;

    auto rc = sqlite3_exec(db, "BEGIN TRANSACTION", nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK)
    {
        std::println(stderr, "SQL error: {}", errMsg);
        sqlite3_free(errMsg);
        return false;
    }

    sqlite3_stmt *stmt_raw = nullptr;
    rc = sqlite3_prepare_v2(db, insertSQL, -1, &stmt_raw, nullptr);
    const std::unique_ptr<sqlite3_stmt, decltype([](sqlite3_stmt *stmt) { sqlite3_finalize(stmt); } )> stmt(stmt_raw);
    if (rc != SQLITE_OK)
    {
        std::println(stderr, "SQL error: {}", sqlite3_errmsg(db));
        return false;
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
        std::println(stderr, "Failed to insert data: {}", sqlite3_errmsg(db));
        sqlite3_exec(db, "ROLLBACK", nullptr, nullptr, &errMsg);
        return false;
    }

    rc = sqlite3_exec(db, "COMMIT", nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK)
    {
        std::println(stderr, "SQL failed to commit transaction: {}", errMsg);
        sqlite3_free(errMsg);
        sqlite3_exec(db, "ROLLBACK", nullptr, nullptr, &errMsg);

        return false;
    }

    return true;
}
