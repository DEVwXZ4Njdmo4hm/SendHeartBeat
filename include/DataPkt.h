#pragma once

#include <array>
#include <string>

#include "Utils.h"

namespace HeartBeat {
    constexpr size_t STRING_SIZE = 64;
    
    template <typename T>
    using serialized_data_t = std::array<std::byte, sizeof(T)>;

    template <size_t SIZE>
    void StringToArray(const std::string& src, std::array<u_char, SIZE>& des)
    {
        des.fill(0);

        auto charsToCopy = std::min(src.size(), SIZE - 1);

        std::copy(std::begin(src), std::begin(src) + charsToCopy, std::begin(des));
        des[charsToCopy] = '\0';
    }

    struct MachineInfo {
        std::array<u_char, STRING_SIZE> boardModel;
        std::array<u_char, STRING_SIZE> boardSerial;
        std::array<u_char, STRING_SIZE> computerModel;
        std::array<u_char, STRING_SIZE> computerSerial;
        std::array<u_char, STRING_SIZE> computerSKU;
        std::array<u_char, STRING_SIZE> computerUUID;
        std::array<u_char, STRING_SIZE> computerFirmwareVersion;
        std::array<u_char, STRING_SIZE> computerFirmwareManufacturer;

        MachineInfo(const std::string& boardModel, const std::string& boardSerial,
                    const std::string& computerModel, const std::string& computerSerial,
                    const std::string& computerSKU, const std::string& computerUUID,
                    const std::string& computerFirmwareVersion, const std::string& computerFirmwareManufacturer)
        {
            StringToArray(boardModel, this->boardModel);
            StringToArray(boardSerial, this->boardSerial);
            StringToArray(computerModel, this->computerModel);
            StringToArray(computerSerial, this->computerSerial);
            StringToArray(computerSKU, this->computerSKU);
            StringToArray(computerUUID, this->computerUUID);
            StringToArray(computerFirmwareVersion, this->computerFirmwareVersion);
            StringToArray(computerFirmwareManufacturer, this->computerFirmwareManufacturer);
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

        CpuInfo(double load, double maxTemp, double avgTemp, double packageTemp,
                double coreVolt, double packagePower, double corePower)
            : load_cpuTotal(load), temp_coreMax(maxTemp), temp_coreAvg(avgTemp),
              temp_cpuPackage(packageTemp), volt_cpuCore(coreVolt),
              pwr_cpuPackage(packagePower), pwr_cpuCore(corePower) {}

        CpuInfo() = default;
    };

    struct GpuInfo {
        double load_gpuCore;

        double clk_gpuCore;

        double temp_gpuCore;

        double pwr_gpuPackage;

        GpuInfo(double load, double clock, double temp, double power)
            : load_gpuCore(load), clk_gpuCore(clock), temp_gpuCore(temp),
              pwr_gpuPackage(power) {}

        GpuInfo() = default;
    };

    struct DataPkt {
        bool HeartBeat;

        MachineInfo machInfo;
        CpuInfo cpuInfo;
        GpuInfo gpuInfo;

        std::chrono::time_point<std::chrono::system_clock> timestamp;

        DataPkt(const bool heartBeat, const MachineInfo& machInfo, const CpuInfo& cpuInfo, const GpuInfo& gpuInfo)
            : HeartBeat(heartBeat), machInfo(machInfo), cpuInfo(cpuInfo), gpuInfo(gpuInfo),
              timestamp(GetCurrentTimestamp()) {}
    };
};