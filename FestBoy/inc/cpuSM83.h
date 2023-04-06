#pragma once
#include "emu_typedefs.h"

namespace gb
{
    // The GameBoy CPU called SM83
    class SM83CPU
    {
    public:
        SM83CPU();
        ~SM83CPU() = default;

        auto read8(const u16& address) -> u8;
        auto read16(const u16& address) -> u16;
        
        auto write8(const u16& address, const u8& data) -> void;
        auto write16(const u16& address, const u16& data) -> void;

    public:
        struct Registers
        {
            union
            {
                struct
                {
                    u8 F;
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