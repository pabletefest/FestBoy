/*
 * Copyright (C) 2023 pabletefest
 *
 * Licensed under GPLv3 or any later version.
 * Refer to the included LICENSE file.
 */

#pragma once
#include "emu_typedefs.h"

#if _DEBUG
#include <fstream>
#endif

namespace gb
{
    class GBConsole;

    enum Flags
    {
        C, H, N, Z
    };

    // The GameBoy CPU called SM83
    class SM83CPU
    {
    public:
        explicit SM83CPU(GBConsole* device);
        ~SM83CPU() = default;

        auto read8(const u16& address) -> u8;
        auto read16(const u16& address) -> u16;
        
        auto write8(const u16& address, const u8& data) -> void;
        auto write16(const u16& address, const u16& data) -> void;

        auto reset() -> void;
        auto clock() -> void;

        constexpr auto getFlag(Flags flag) -> u8
        {
            switch (flag)
            {
            case C:
                return regs.flags.C;
                break;
            case H:
                return regs.flags.H;
                break;
            case N:
                return regs.flags.N;
                break;
            case Z:
                return regs.flags.Z;
                break;
            default:
                return 0;
                break;
            }
        }

        constexpr auto setFlag(Flags flag, u8 value) -> void
        {
            switch (flag)
            {
            case C:
                regs.flags.C = value ? 1 : 0;
                break;
            case H:
                regs.flags.H = value ? 1 : 0;
                break;
            case N:
                regs.flags.N = value ? 1 : 0;
                break;
            case Z:
                regs.flags.Z = value ? 1 : 0;
                break;
            default:
                break;
            }
        }

        auto checkPendingInterrupts() -> bool;

    private:
        auto decodeAndExecuteInstruction(u8 opcode) -> void;
        auto decodeAndExecuteCBInstruction(u8 cbOpcode) -> void;

        auto setRegisterValuesPostBootROM() -> void;

    private:
        u8 instructionCycles = 0;
        u64 cpuT_CyclesElapsed = 0;
        u64 cpuM_CyclesElapsed = 0;

#if /*_DEBUG*/ 0
        std::ofstream logFile;
#endif

    public:
        GBConsole* system = nullptr;

        struct Registers
        {
            union
            {
                struct
                {
                    union
                    {
                        struct
                        {
                            u8 unused : 4;
                            u8 C : 1;
                            u8 H : 1;
                            u8 N : 1;
                            u8 Z : 1;
                        }flags;

                        u8 F;
                    };

                    u8 A;
                };

                u16 AF;
            };

            union
            {
                struct
                {
                    u8 C;
                    u8 B;
                };

                u16 BC;
            };

            union
            {
                struct
                {
                    u8 E;
                    u8 D;
                };

                u16 DE;
            };

            union
            {
                struct
                {
                    u8 L;
                    u8 H;
                };

                u16 HL;
            };

            u16 SP;
            u16 PC;
        }regs;
        
        u8* debugRAM = nullptr;
    };
}