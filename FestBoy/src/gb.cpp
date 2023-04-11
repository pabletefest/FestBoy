/*
 * Copyright (C) 2023 pabletefest
 *
 * Licensed under GPLv3 or any later version.
 * Refer to the included LICENSE file.
 */

#pragma once
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

    //return (internalRAM[address + 1] << 8) | internalRAM[address];

    return (read8(address + 1) << 8) | read8(address);
}

auto gb::GBConsole::write8(const u16& address, const u8& data) -> void
{
    switch (address)
    {
    case 0xFFFF:
        IE.reg = data;
        break;
    case 0xFF0F:
        IF.reg = data;
        break;
    default:
        internalRAM[address] = data;
        break;
    }
}

auto gb::GBConsole::write16(const u16& address, const u16& data) -> void
{
    /* ----- ALTERNATIVE -----
        u16* arrPtr = reinterpret_cast<u16*>(&internalRAM[address]);
        *arrPtr = data;
    */

    /*internalRAM[address] = static_cast<u8>(data & 0x00FF);
    internalRAM[address + 1] = static_cast<u8>((data >> 8) & 0x00FF);*/

    write8(address, static_cast<u8>(data & 0x00FF));
    write8(address + 1, static_cast<u8>((data >> 8) & 0x00FF));
}

auto gb::GBConsole::reset() -> void
{
    cpu.reset();
}

auto gb::GBConsole::clock() -> void
{
    cpu.clock();

    systemCyclesElapsed++;
}

auto gb::GBConsole::step(u64 numberCycles) -> void
{
    for (size_t i = 0; i < numberCycles; i++)
        clock();
}
