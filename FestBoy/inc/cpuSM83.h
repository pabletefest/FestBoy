#pragma once
#include "emu_typedefs.h"

namespace gb
{
    class GBConsole;

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

    private:
        auto decodeAndExecuteInstruction(u8 opcode) -> void;
        auto decodeAndExecuteCBInstruction(u8 cbOpcode) -> void;

    private:
        GBConsole* system = nullptr;
        u8 instructionCycles = 0;
        u64 cpuCyclesElapsed = 0;

    public:
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
    };
}