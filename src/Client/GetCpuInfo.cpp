#include "Client.h"

void HeartBeat::GetCpuInfo(CpuInfo& cpuInfo)
{
    if (not cpuSensorIDInitialized)
    {
        std::println(stderr, "Error: CPU Sensor ID not initialized.");
        return;
    }

#ifdef _WIN32

    cpuInfo.load_cpuTotal = LHWM::GetSensorValue(path_load_cpuTotal);
    cpuInfo.temp_coreMax = LHWM::GetSensorValue(path_temp_coreMax);
    cpuInfo.temp_coreAvg = LHWM::GetSensorValue(path_temp_coreAvg);
    cpuInfo.temp_cpuPackage = LHWM::GetSensorValue(path_temp_cpuPackage);
    cpuInfo.volt_cpuCore = LHWM::GetSensorValue(path_volt_cpuCore);
    cpuInfo.pwr_cpuPackage = LHWM::GetSensorValue(path_pwr_cpuPackage);
    cpuInfo.pwr_cpuCore = LHWM::GetSensorValue(path_pwr_cpuCore);

#elifdef __linux__

    std::ifstream cpuInfoFile("/sys/class/thermal/thermal_zone0/temp");
    if (cpuInfoFile.is_open())
    {
        std::string line;
        std::getline(cpuInfoFile, line);
        cpuInfo.temp_cpuPackage = std::stod(line) / 1000.0;
        cpuInfoFile.close();
    }

    // For CPU load, voltage and power, you may need to use other methods or libraries.
    // This is just a placeholder.
    cpuInfo.load_cpuTotal = 0.0; // Placeholder
    cpuInfo.volt_cpuCore = 0.0;  // Placeholder
    cpuInfo.pwr_cpuPackage = 0.0; // Placeholder

#endif
}