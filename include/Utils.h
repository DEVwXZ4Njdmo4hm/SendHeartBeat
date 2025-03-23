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

    template <size_t SIZE>
    void StringToArray(const std::string& src, std::array<u_char, SIZE>& des)
    {
        des.fill(0);

        size_t charsToCopy = std::min(src.size(), SIZE - 1);

        std::copy(std::begin(src), std::begin(src) + charsToCopy, std::begin(des));
        des[charsToCopy] = '\0';
    }

    template <size_t SIZE>
    std::string UCharArrayToString(const std::array<u_char, SIZE>& arr)
    {
        return std::string(reinterpret_cast<const char*>(arr.data()));
    }

    inline std::chrono::time_point<std::chrono::system_clock> GetCurrentTimestamp() {
        return std::chrono::system_clock::now();
    }
}