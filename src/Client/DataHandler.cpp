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

    GetCpuSensorID();
    GetGpuSensorID();
    DataPkt dataPkt;
    GetCpuInfo(dataPkt.cpuInfo);
    GetGpuInfo(dataPkt.gpuInfo);
    dataPkt.machInfo = machInfo;
    dataPkt.timestamp = LL_GetCurrentTimeStamp();

    while (worker_signal)
    {
        serialized_data_t<DataPkt> serialized;

        SerializeStruct(serialized, dataPkt);
        Sender(server, port, serialized);

        std::println(stdout, "Data sent to server: {}:{}", server, port);

        std::thread cpuInfoThread(GetCpuInfo, std::ref(dataPkt.cpuInfo));
        std::thread gpuInfoThread(GetGpuInfo, std::ref(dataPkt.gpuInfo));
        dataPkt.timestamp = LL_GetCurrentTimeStamp();

        std::this_thread::sleep_for(std::chrono::seconds(sampling_interval));

        if (cpuInfoThread.joinable())
        {
            cpuInfoThread.join();
        }

        if (gpuInfoThread.joinable())
        {
            gpuInfoThread.join();
        }
    }
}

