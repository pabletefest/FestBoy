/*
 * Copyright (C) 2023 pabletefest
 *
 * Licensed under GPLv3 or any later version.
 * Refer to the included LICENSE file.
 */

#pragma once
#include "gb.h"
#include "game_pack.h"

#include <iostream>

gb::GBConsole::GBConsole()
    : cpu(this), IE({}), IF({})
{
    /*for (auto& item : internalRAM)
        item = 0x00;*/

    std::memset(internalRAM.data(), 0x00, internalRAM.size());
    cpu.debugRAM = internalRAM.data();
}

auto gb::GBConsole::insertCartridge(const Ref<GamePak>& cartridge) -> void
{
    this->gamePak = cartridge;
}

auto gb::GBConsole::read8(const u16& address) -> u8
{
    u8 dataRead = 0x00;

    if (gamePak->read(address, dataRead))
    {
        // Let the Cartridge handle the read
    }
    else
    {
        switch (address)
        {
        case 0xFFFF:
            dataRead = 0xE0 | IE.reg;
            break;
        case 0xFF0F:
            dataRead = 0xE0 | IF.reg;
            break;
        default:
            dataRead = internalRAM[address];
            break;
        }
    }

    return dataRead;
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
    case 0xFF02:
        if (data == 0x81)
            printf("%c", internalRAM[0xFF01]);
        break;
    case 0xFFFF:
        IE.reg = data;
        break;
    case 0xFF0F:
        IF.reg = data;
        break;
    default:
        if (gamePak->write(address, data))
        {
            // Let the cartridge handle the write
        }
        else
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

    // TEMP for tests
    cpu.regs.PC = 0x0100;
    internalRAM[0xFF44] = 0x90;
}

auto gb::GBConsole::clock() -> void
{
    /*if (IME && (IF.reg & IE.reg & 0x1F))
    {
        pendingInterrupt = true;
    }*/

    /*if (IME && (IF.reg & IE.reg & 0x1F))
    {
        cpu.interruptServiceRoutine();
    }
    else
    {
        cpu.clock();
    }*/

    cpu.clock();

    systemCyclesElapsed++;
}

auto gb::GBConsole::step(u64 numberCycles) -> void
{
    for (size_t i = 0; i < numberCycles; i++)
        clock();
}
