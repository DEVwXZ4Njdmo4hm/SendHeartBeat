#include "Server.h"

using namespace HeartBeat;

void HeartBeat::DataHandler()
{
    while (work_signal)
    {
        if (not msg_queue.empty())
        {
            serialized_data_t<DataPkt> buffer;

            {
                std::lock_guard lock(msg_queue_mutex);
                buffer = msg_queue.front();
                msg_queue.pop_front();
            }

            DataPkt msg {};
            DeserializeStruct(buffer, msg);

            WriteToDB(msg);
        }
    }
}
