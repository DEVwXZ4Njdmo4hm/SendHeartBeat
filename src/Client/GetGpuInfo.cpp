#include "Client.h"

void HeartBeat::GetGpuInfo(GpuInfo& gpuInfo)
{
    if (not gpuSensorIDInitialized)
    {
        std::println(stderr, "Error: GPU Sensor ID not initialized.");
        return;
    }

#ifdef _WIN32

    gpuInfo.load_gpuCore = LHWM::GetSensorValue(path_load_gpuCore);
    gpuInfo.clk_gpuCore = LHWM::GetSensorValue(path_clk_gpuCore);
    gpuInfo.temp_gpuCore = LHWM::GetSensorValue(path_temp_gpuCore);
    gpuInfo.pwr_gpuPackage = LHWM::GetSensorValue(path_pwr_gpuPackage);

#endif
}