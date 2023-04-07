#include "gb.h"

gb::GBConsole::GBConsole()
    : cpu(this)
{
    /*for (auto& item : internalRAM)
        item = 0x00;*/

    std::memset(internalRAM.data(), 0x00, internalRAM.size());
}

auto gb::GBConsole::read8(const u16& address) -> u8
{
    return internalRAM[address];
}

auto gb::GBConsole::read16(const u16& address) -> u16
{
    /* ----- ALTERNATIVE -----
        u16* arrPtr = reinterpret_cast<u16*>(&internalRAM[address]);
        return *arrPtr;
    */

    return (internalRAM[address + 1] << 8) | internalRAM[address];
}

auto gb::GBConsole::write8(const u16& address, const u8& data) -> void
{
    internalRAM[address] = data;
}

auto gb::GBConsole::write16(const u16& address, const u16& data) -> void
{
    /* ----- ALTERNATIVE -----
        u16* arrPtr = reinterpret_cast<u16*>(&internalRAM[address]);
        *arrPtr = data;
    */

    internalRAM[address] = static_cast<u8>(data & 0x00FF);
    internalRAM[address + 1] = static_cast<u8>((data >> 8) & 0x00FF);
}
