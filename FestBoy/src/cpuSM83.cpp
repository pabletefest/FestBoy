#include "cpuSM83.h"

gb::SM83CPU::SM83CPU()
    : regs({})
{
}

auto gb::SM83CPU::read8(const u16& address) -> u8
{
    return u8();
}

auto gb::SM83CPU::read16(const u16& address) -> u16
{
    return u16();
}

auto gb::SM83CPU::write8(const u16& address, const u8& data) -> void
{
}

auto gb::SM83CPU::write16(const u16& address, const u16& data) -> void
{
}
