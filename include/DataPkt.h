#pragma once

#include <array>
#include <string>

namespace HeartBeat {
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
        bool HeartBeat;

        std::string boardModel;
        std::string boardSerial;
        std::string computerModel;
        std::string computerSerial;
        std::string computerSKU;
        std::string computerUUID;
        std::string computerFirmwareVersion;
        std::string computerFirmwareManufacturer;

        CpuInfo cpuInfo;
        GpuInfo gpuInfo;

        std::chrono::time_point<std::chrono::system_clock> timestamp;
    };
};