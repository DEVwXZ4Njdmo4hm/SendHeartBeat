#include "Server.h"

using namespace HeartBeat;

void HeartBeat::DataHandler()
{
    while (work_signal)
    {
        if (not msg_queue.empty())
        {
            {
                std::lock_guard lock(msg_queue_mutex);
                wait_queue.push(msg_queue.front());
                msg_queue.pop_front();
            }
        }

        auto retries = 0, max_retries = 10;

        while (not wait_queue.empty())
        {
            if (retries > max_retries and work_signal)
                break;

            DataPkt msg {};
            DeserializeStruct(wait_queue.front(), msg);

            if (WriteToDB(msg))
                wait_queue.pop();
            else
                retries += 1;

        }
    }
}
