#pragma once

#include <array>
#include <print>
#include <string>

#include "Utils.h"

namespace HeartBeat {
    constexpr size_t STRING_SIZE = 64;

    template <typename T>
    using serialized_data_t = std::array<std::byte, sizeof(T)>;

#pragma pack(push, 1)
    struct MachineInfo {
        std::array<u_char, STRING_SIZE> boardModel;
        std::array<u_char, STRING_SIZE> boardSerial;
        std::array<u_char, STRING_SIZE> computerModel;
        std::array<u_char, STRING_SIZE> computerSerial;
        std::array<u_char, STRING_SIZE> computerSKU;
        std::array<u_char, STRING_SIZE> computerUUID;
        std::array<u_char, STRING_SIZE> computerFirmwareVersion;
        std::array<u_char, STRING_SIZE> computerFirmwareManufacturer;

        static void set(
            MachineInfo& machInfo,
            const std::string& boardModel, const std::string& boardSerial,
            const std::string& computerModel, const std::string& computerSerial,
            const std::string& computerSKU, const std::string& computerUUID,
            const std::string& computerFirmwareVersion, const std::string& computerFirmwareManufacturer)
        {
            StringToArray(boardModel, machInfo.boardModel);
            StringToArray(boardSerial, machInfo.boardSerial);
            StringToArray(computerModel, machInfo.computerModel);
            StringToArray(computerSerial, machInfo.computerSerial);
            StringToArray(computerSKU, machInfo.computerSKU);
            StringToArray(computerUUID, machInfo.computerUUID);
            StringToArray(computerFirmwareVersion, machInfo.computerFirmwareVersion);
            StringToArray(computerFirmwareManufacturer, machInfo.computerFirmwareManufacturer);
        }
    };

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
        int64_t timestamp;

        MachineInfo machInfo{};
        CpuInfo cpuInfo{};
        GpuInfo gpuInfo{};
    };
#pragma pack(pop)

    // Write assertions to check the size of the struct
    static_assert(sizeof(MachineInfo) == 8 * STRING_SIZE, "MachineInfo size mismatch");
    static_assert(sizeof(CpuInfo) == 7 * sizeof(double), "CpuInfo size mismatch");
    static_assert(sizeof(GpuInfo) == 4 * sizeof(double), "GpuInfo size mismatch");
    static_assert(sizeof(DataPkt) == sizeof(int64_t) + sizeof(MachineInfo) + sizeof(CpuInfo) + sizeof(GpuInfo), "DataPkt size mismatch");
};