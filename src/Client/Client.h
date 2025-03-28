#pragma once

#include <print>
#include <regex>
#include <ranges>
#include <string>
#include <thread>
#include <iostream>
#include <asio.hpp>

#include <config.h>

#if defined(_WIN32)

#include <lhwm-cpp-wrapper.h>
#include <comdef.h>
#include <WbemIdl.h>
#pragma comment(lib, "wbemuuid.lib")

#elif defined(__linux__)

#include <fstream>
#include <filesystem>

#endif

#include <DataPkt.h>

namespace HeartBeat {
    inline MachineInfo machInfo {};

    inline std::string boardModel = "Unknown";
    inline std::string boardSerial = "Unknown";
    inline std::string computerModel = "Unknown";
    inline std::string computerSerial = "Unknown";
    inline std::string computerSKU = "Unknown";
    inline std::string computerUUID = "Unknown";
    inline std::string computerFirmwareVersion = "Unknown";
    inline std::string computerFirmwareManufacturer = "Unknown";

    inline std::string path_load_cpuTotal;
    inline std::string path_temp_coreMax;
    inline std::string path_temp_coreAvg;
    inline std::string path_temp_cpuPackage;
    inline std::string path_volt_cpuCore;
    inline std::string path_pwr_cpuPackage;
    inline std::string path_pwr_cpuCore;
    inline std::string path_load_gpuCore;
    inline std::string path_clk_gpuCore;
    inline std::string path_temp_gpuCore;
    inline std::string path_pwr_gpuPackage;

    inline std::atomic_bool worker_signal { true };

#ifdef _WIN32
    inline std::map<std::string, std::vector<std::tuple<std::string, std::string, std::string>>> sensorMap;
    inline bool sensorMapInitialized = false;

    inline void InitSensorMap() {
        sensorMap = LHWM::GetHardwareSensorMap();
        sensorMapInitialized = true;
    }

    /*
    // Copilot: Write a function for me that retrieves the sensor map from LHWM library.
    inline void RetriveSensorMap()
    {
        if (not sensorMapInitialized)
        {
            InitSensorMap();
        }

        for (const auto& [categrory, sensors] : sensorMap)
        {
            std::println("Category: {}", categrory);
            for (const auto& [name, type, id] : sensors)
            {
                std::println("Name: {}, Type: {}, ID: {}", name, type, id);
            }
        }
    }
    */
#endif

    inline bool machModelInitialized = false;
    void GetMachModel();

    inline bool cpuSensorIDInitialized = false;
    /**
     *  Retrieves the CPU sensor ID using the LHWM library.
     *  This function is called once to initialize the CPU sensor ID.
     *  The CPU sensor ID is used to retrieve the CPU temperature.
     */
    void GetCpuSensorID();

    inline bool gpuSensorIDInitialized = false;
    /**
     *  Retrieves the NVIDIA GPU sensor ID using the LHWM library.
     *  This function is called once to initialize the GPU sensor ID.
     *  The GPU sensor ID is used to retrieve the GPU temperature.
     */
    void GetGpuSensorID();

    /**
     *  Retrieves the CPU temperature using the LHWM library.
     *  Return CpuInfo struct.
     */
    void GetCpuInfo(CpuInfo& cpuInfo);

    /**
     *  Retrieves the NVIDIA dGPU temperature using the LHWM library.
     *  Return dGPU Package temperature.
     */
    void GetGpuInfo(GpuInfo& gpuInfo);

    /**
     *  Sends the serialized data to the server using ASIO library.
     *  @param server The server address to send the data to.
     *  @param port The server port to send the data to.
     *  @param data The serialized data to be sent.
     */
    void Sender(std::string server, port_t port, serialized_data_t<DataPkt>& data);

    void DataHandler(std::string server, port_t port);

    inline bool IsValidIPv4(const std::string& ip)
    {
        try {
            auto address = asio::ip::make_address(ip);
            return address.is_v4();
        }
        catch (const std::exception& e) {
            return false;
        }
    }
}