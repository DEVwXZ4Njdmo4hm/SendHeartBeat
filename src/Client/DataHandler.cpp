#include "Client.h"

using namespace HeartBeat;

void HeartBeat::DataHandler(std::string server, port_t port)
{
#ifdef _WIN32

    InitSensorMap();

    std::println("Sensor Map Size: {}", sensorMap.size());
#endif

    GetMachModel();

    MachineInfo::set(
        machInfo,
        boardModel, boardSerial,
        computerModel, computerSerial,
        computerSKU, computerUUID,
        computerFirmwareVersion, computerFirmwareManufacturer
    );

    // print all information about the machine
    std::println("\n\n");
    std::println("Motherboard Model: {}", UCharArrayToString(machInfo.boardModel));
    std::println("Motherboard Serial: {}", UCharArrayToString(machInfo.boardSerial));
    std::println("Computer Model: {}", UCharArrayToString(machInfo.computerModel));
    std::println("Computer Serial: {}", UCharArrayToString(machInfo.computerSerial));
    std::println("Computer SKU: {}", UCharArrayToString(machInfo.computerSKU));
    std::println("Computer UUID: {}", UCharArrayToString(machInfo.computerUUID));
    std::println("Computer Firmware Version: {}", UCharArrayToString(machInfo.computerFirmwareVersion));
    std::println("Computer Firmware Manufacturer: {}", UCharArrayToString(machInfo.computerFirmwareManufacturer));
    std::println("\n\n");

    CpuInfo cpuTemp {};
    GpuInfo gpuTemp {};

    GetCpuSensorID();
    GetGpuSensorID();

    while (worker_signal)
    {
        GetCpuInfo(cpuTemp);
        GetGpuInfo(gpuTemp);

        DataPkt dataPkt;
        dataPkt.cpuInfo = cpuTemp;
        dataPkt.gpuInfo = gpuTemp;
        dataPkt.machInfo = machInfo;
        dataPkt.HeartBeat = true;
        dataPkt.timestamp = GetCurrentTimestamp();

        serialized_data_t<DataPkt> serialized;

        SerializeStruct(serialized, dataPkt);
        Sender(server, port, serialized);

        std::println(stdout, "Data sent to server: {}:{}", server, port);

        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
}

