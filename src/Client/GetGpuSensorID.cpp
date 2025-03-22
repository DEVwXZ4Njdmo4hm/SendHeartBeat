#include "Client.h"

using namespace HeartBeat;

void HeartBeat::GetGpuSensorID()
{
#ifdef _WIN32
    for (const auto& [category, sensors] : sensorMap) {
        if (std::regex_search(category, std::regex("NVIDIA.*GPU")))
        {
            for (const auto& [name, type, id] : sensors)
            {
                if (name == "GPU Core" && type == "Load")
                {
                    path_load_gpuCore = id;
                    continue;
                }

                if (name == "GPU Core" && type == "Clock")
                {
                    path_clk_gpuCore = id;
                    continue;
                }

                if (name == "GPU Core" and type == "Temperature")
                {
                    path_temp_gpuCore = id;
                    continue;
                }

                if (name == "GPU Package" && type == "Power")
                {
                    path_pwr_gpuPackage = id;
                }
            }
        }
    }

    gpuSensorIDInitialized = true;
#endif
}