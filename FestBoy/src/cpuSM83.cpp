#include "cpuSM83.h"
#include "gb.h"

gb::SM83CPU::SM83CPU(GBConsole* device)
    : system(device), regs({})
{
}

auto gb::SM83CPU::read8(const u16& address) -> u8
{
    return system->read8(address);
}

auto gb::SM83CPU::read16(const u16& address) -> u16
{
    return system->read16(address);
}

auto gb::SM83CPU::write8(const u16& address, const u8& data) -> void
{
    system->write8(address, data);
}

auto gb::SM83CPU::write16(const u16& address, const u16& data) -> void
{
    system->write16(address, data);
}
