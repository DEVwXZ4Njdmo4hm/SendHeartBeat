#include "Client.h"

using namespace HeartBeat;

void HeartBeat::GetCpuSensorID()
{
#ifdef _WIN32
    for (const auto& [category, sensors] : sensorMap) {
        if (category.contains("Intel Core"))
        {
            for (const auto& [name, type, id] : sensors)
            {
                if (name == "CPU Total" && type == "Load")
                {
                    path_load_cpuTotal = id;
                    continue;
                }

                if (name == "Core Max" && type == "Temperature")
                {
                    path_temp_coreMax = id;
                    continue;
                }

                if (name == "Core Average" && type == "Temperature")
                {
                    path_temp_coreAvg = id;
                    continue;
                }

                if (name == "CPU Package" && type == "Temperature")
                {
                    path_temp_cpuPackage = id;
                    continue;
                }

                if (name == "CPU Core" && type == "Voltage")
                {
                    path_volt_cpuCore = id;
                    continue;
                }

                if (name == "CPU Package" && type == "Power")
                {
                    path_pwr_cpuPackage = id;
                }

                if (name == "CPU Cores" && type == "Power")
                {
                    path_pwr_cpuCore = id;
                }
            }
        }
    };

    cpuSensorIDInitialized = true;
#endif
}
