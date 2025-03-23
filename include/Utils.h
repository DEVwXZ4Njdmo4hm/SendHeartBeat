#pragma once

#include <array>
#include <memory>

#include <pfr.hpp>

namespace HeartBeat {

    using port_t = uint16_t;

    template <typename T>
    void SerializeStruct(std::array<std::byte, sizeof(T)>& buffer, const T& obj)
    {
        size_t offset = 0;

        pfr::for_each_field(
            obj,
            [&buffer, &offset](auto& member) {
                std::memcpy(
                    buffer.data() + offset,
                    &member,
                    sizeof(member)
                );

                offset += sizeof(member);
            }
        );
    }

    template <typename T>
    void DeserializeStruct(const std::array<std::byte, sizeof(T)>& buffer, T& obj)
    {
        size_t offset = 0;

        pfr::for_each_field(
            obj,
            [&buffer, &offset](auto& member) {
                std::memcpy(
                    &member,
                    buffer.data() + offset,
                    sizeof(member)
                );

                offset += sizeof(member);
            }
        );
    }

    inline std::chrono::time_point<std::chrono::system_clock> GetCurrentTimestamp() {
        return std::chrono::system_clock::now();
    }
}