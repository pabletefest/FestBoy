/*
 * Copyright (C) 2023 pabletefest
 *
 * Licensed under GPLv3 or any later version.
 * Refer to the included LICENSE file.
 */

#pragma once
#include "emu_typedefs.h"
#include "cpu_sm83.h"
#include "game_pack.h"

#include <array>

namespace gb
{
    class GBConsole
    {
    public:
        GBConsole();
        ~GBConsole() = default;

        auto insertCartridge(const Ref<GamePak>& cartridge) -> void;

        auto read8(const u16& address) -> u8;
        auto read16(const u16& address) -> u16;

        auto write8(const u16& address, const u8& data) -> void;
        auto write16(const u16& address, const u16& data) -> void;

        auto reset() -> void;
        auto clock() -> void;
        auto step(u64 numberCycles) -> void;

        inline auto getCPU() -> SM83CPU&
        {
            return cpu;
        }

    private:
        SM83CPU cpu;
        std::array<u8, convertKBToBytes(8)> wram;

        u64 systemCyclesElapsed = 0;

        Ref<GamePak> gamePak;

        u8 SB_register; // Serial transfer data register
        u8 SC_register; // Serial transfer control register

    public:
        bool IME = false;
        bool pendingInterrupt = false;

        union InterruptEnableRegister
        {
            struct
            {
                u8 VBlank : 1;
                u8 LCD_STAT : 1;
                u8 Timer : 1;
                u8 Serial : 1;
                u8 Joypad : 1;
                u8 unused : 3;
            };

            u8 reg;
        }IE;

        union InterruptFlagsRegister
        {
            struct
            {
                u8 VBlank : 1;
                u8 LCD_STAT : 1;
                u8 Timer : 1;
                u8 Serial : 1;
                u8 Joypad : 1;
                u8 unused : 3;
            };

            u8 reg;
        }IF;
    };
}