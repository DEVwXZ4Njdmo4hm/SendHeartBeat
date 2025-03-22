#include "Client.h"

using namespace HeartBeat;

int main()
{
    std::println("Hello, this is Client!");
    std::println("ASIO Version: {}", ASIO_VERSION);
    std::println("PFR Version: {}", PFR_VERSION);

#ifdef _WIN32

    InitSensorMap();

    std::println("Sensor Map Size: {}", sensorMap.size());
#endif

    GetMachModel();

    // print all information about the machine
    std::println("\n\n");
    std::println("Motherboard Model: {}", boardModel);
    std::println("Motherboard Serial: {}", boardSerial);
    std::println("Computer Model: {}", computerModel);
    std::println("Computer Serial: {}", computerSerial);
    std::println("Computer SKU: {}", computerSKU);
    std::println("Computer UUID: {}", computerUUID);
    std::println("Computer Firmware Version: {}", computerFirmwareVersion);
    std::println("Computer Firmware Manufacturer: {}", computerFirmwareManufacturer);

    std::println("\n\n");

    CpuInfo cpuTemp {};
    GpuInfo gpuTemp {};

    GetCpuSensorID();
    GetGpuSensorID();

    for ([[maybe_unused]] auto _ : std::views::iota(0, 10))
    {
        GetCpuInfo(cpuTemp);
        GetGpuInfo(gpuTemp);

        std::println(
            "CPU Load: {:.2f}%, "
            "CPU Max Temp : {:.2f}°C, CPU Average Temp: {:.2f}°C, CPU Package Temp: {:.2f}°C, "
            "CPU Core Volt: {:.2f}V, CPU Package Power: {:.2f}W, CPU Core Power: {:.2f}W",
            cpuTemp.load_cpuTotal,
            cpuTemp.temp_coreMax,
            cpuTemp.temp_coreAvg,
            cpuTemp.temp_cpuPackage,
            cpuTemp.volt_cpuCore,
            cpuTemp.pwr_cpuPackage,
            cpuTemp.pwr_cpuCore
        );

        std::println(
            "GPU Load: {:.2f}%, GPU Core Clock: {:.2f}MHz, GPU Core Temp: {:.2f}°C, GPU Package Power: {:.2f}W",
            gpuTemp.load_gpuCore,
            gpuTemp.clk_gpuCore,
            gpuTemp.temp_gpuCore,
            gpuTemp.pwr_gpuPackage
        );

        std::println("\n");

        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    return 0;
}
