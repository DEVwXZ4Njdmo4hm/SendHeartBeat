#pragma once

#include <array>
#include <string>

namespace HeartBeat {
    constexpr size_t STRING_SIZE = 64;
    
    template <typename T>
    using serialized_data_t = std::array<std::byte, sizeof(T)>;
    

    struct CpuInfo {
        double load_cpuTotal;

        double temp_coreMax;
        double temp_coreAvg;
        double temp_cpuPackage;

        double volt_cpuCore;

        double pwr_cpuPackage;
        double pwr_cpuCore;
    };

    struct GpuInfo {
        double load_gpuCore;

        double clk_gpuCore;

        double temp_gpuCore;

        double pwr_gpuPackage;
    };

    struct DataPkt {
        bool HeartBeat {};

        std::array<u_char, STRING_SIZE> boardModel {};
        std::array<u_char, STRING_SIZE> boardSerial {};
        std::array<u_char, STRING_SIZE> computerModel {};
        std::array<u_char, STRING_SIZE> computerSerial {};
        std::array<u_char, STRING_SIZE> computerSKU {};
        std::array<u_char, STRING_SIZE> computerUUID {};
        std::array<u_char, STRING_SIZE> computerFirmwareVersion {};
        std::array<u_char, STRING_SIZE> computerFirmwareManufacturer {};

        CpuInfo cpuInfo;
        GpuInfo gpuInfo;

        std::chrono::time_point<std::chrono::system_clock> timestamp;
    };
};