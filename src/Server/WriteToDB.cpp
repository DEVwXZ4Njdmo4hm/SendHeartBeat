#include "Server.h"

using namespace HeartBeat;

void HeartBeat::WriteToDB(DataPkt &data)
{
    // just print the data for now
    std::println("Writing to DB:");
    std::println("  HeartBeat: {}", data.HeartBeat);
    std::println("  Board Model: {}", UCharArrayToString(data.machInfo.boardModel));
    std::println("  Board Serial: {}", UCharArrayToString(data.machInfo.boardSerial));
    std::println("  Computer Model: {}", UCharArrayToString(data.machInfo.computerModel));
    std::println("  Computer Serial: {}", UCharArrayToString(data.machInfo.computerSerial));
    std::println("  Computer SKU: {}", UCharArrayToString(data.machInfo.computerSKU));
    std::println("  Computer UUID: {}", UCharArrayToString(data.machInfo.computerUUID));
    std::println("  Computer Firmware Version: {}", UCharArrayToString(data.machInfo.computerFirmwareVersion));
    std::println("  Computer Firmware Manufacturer: {}", UCharArrayToString(data.machInfo.computerFirmwareManufacturer));
    std::println("  CPU Load: {}", data.cpuInfo.load_cpuTotal);
    std::println("  CPU Temp Max: {}", data.cpuInfo.temp_coreMax);
    std::println("  CPU Temp Avg: {}", data.cpuInfo.temp_coreAvg);
    std::println("  CPU Temp Package: {}", data.cpuInfo.temp_cpuPackage);
    std::println("  CPU Voltage: {}", data.cpuInfo.volt_cpuCore);
    std::println("  CPU Power Package: {}", data.cpuInfo.pwr_cpuPackage);
    std::println("  CPU Power Core: {}", data.cpuInfo.pwr_cpuCore);
    std::println("  GPU Load: {}", data.gpuInfo.load_gpuCore);
    std::println("  GPU Clock: {}", data.gpuInfo.clk_gpuCore);
    std::println("  GPU Temp: {}", data.gpuInfo.temp_gpuCore);
    std::println("  GPU Power: {}", data.gpuInfo.pwr_gpuPackage);
    std::println("  Timestamp: {}", data.timestamp.time_since_epoch().count());
}
