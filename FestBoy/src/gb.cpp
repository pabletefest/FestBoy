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

    std::memset(wram.data(), 0x00, wram.size());
    //cpu.debugRAM = internalRAM.data();
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
    else if (address >= 0x8000 && address <= 0x9FFF)
    {

    }
    else if (address >= 0xA000 && address <= 0xBFFF)
    {

    }
    else if (address >= 0xC000 && address <= 0xDFFF)
    {
        dataRead = wram[address & 0x1FFF];
    }
    else if (address >= 0xE000 && address <= 0xFDFF) // (ECHO RAM)
    {
        dataRead = wram[address & 0x1DFF];
    }
    else if (address >= 0xFF00 && address <= 0xFF7F)// IO Registers
    {
        switch (address)
        {
        case 0xFF01:
            dataRead = SB_register;
            break;
        case 0xFF02:
            dataRead = SC_register;
            break;
        case 0xFF0F:
            dataRead = 0xE0 | IF.reg;
            break;
        case 0xFF44:
            dataRead = 0x90;
            break;
        /*default:
            dataRead = internalRAM[address];
            break;*/
        }
    }
    else if (address == 0xFFFF)
    {
        dataRead = 0xE0 | IE.reg;
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
    if (gamePak->write(address, data))
    {
        // Let the cartridge handle the write
    }
    else if (address >= 0x8000 && address <= 0x9FFF)
    {

    }
    else if (address >= 0xA000 && address <= 0xBFFF)
    {

    }
    else if (address >= 0xC000 && address <= 0xDFFF)
    {
        wram[address & 0x1FFF] = data;
    }
    else if (address >= 0xE000 && address <= 0xFDFF) // (ECHO RAM)
    {
        wram[address & 0x1DFF] = data;
    }
    else if (address >= 0xFF00 && address <= 0xFF7F)// IO Registers
    {
        switch (address)
        {
        case 0xFF01:
            SC_register = data;
            break;
        case 0xFF02:
            if (data == 0x81)
                printf("%c", /*internalRAM[0xFF01]*/ SB_register);
            break;
        case 0xFF0F:
            IF.reg = data;
            break;
        }
    }
    else if (address == 0xFFFF)
    {
        IE.reg = data;
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
    //internalRAM[0xFF44] = 0x90;
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
