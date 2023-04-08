#pragma once
#include "emu_typedefs.h"
#include "cpuSM83.h"

#include <array>

namespace gb
{
    class GBConsole
    {
    public:
        GBConsole();
        ~GBConsole() = default;

        auto read8(const u16& address) -> u8;
        auto read16(const u16& address) -> u16;

        auto write8(const u16& address, const u8& data) -> void;
        auto write16(const u16& address, const u16& data) -> void;

        auto reset() -> void;
        auto clock() -> void;
        auto step(u64 numberCycles) -> void;

    private:
        SM83CPU cpu;
        std::array<u8, convertKBToBytes(64)> internalRAM;

        u64 systemCyclesElapsed = 0;
    };
}