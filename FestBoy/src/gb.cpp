/*
 * Copyright (C) 2023 pabletefest
 *
 * Licensed under GPLv3 or any later version.
 * Refer to the included LICENSE file.
 */

#pragma once
#include "gb.h"
#include "bootrom.h"
#include "game_pack.h"

#include <iostream>

gb::GBConsole::GBConsole()
    : cpu(this), IE({}), IF({}), timer(this), ppu(this)
{
    /*for (auto& item : internalRAM)
        item = 0x00;*/

    std::memset(wram.data(), 0x00, wram.size());
    std::memset(hram.data(), 0x00, hram.size());
    std::memset(internalRAM.data(), 0x00, internalRAM.size());
    //cpu.debugRAM = internalRAM.data();

    cpu.reset();
    ppu.reset();
}

auto gb::GBConsole::insertCartridge(const Ref<GamePak>& cartridge) -> void
{
    this->gamePak = cartridge;
}

auto gb::GBConsole::read8(const u16& address) -> u8
{
    u8 dataRead = 0x00;

    if (address < 0x0100 && ((bootROMMappedRegister & 0x01) == 0))
    {
        // BootROM is mapped in the first 256 bytes of address space so PC points to this code
        dataRead = boot_rom[address & 0xFF];
        //dataRead = bootROMFromFile[address & 0xFF];
    }
    else if (gamePak->read(address, dataRead))
    {
        // Let the Cartridge handle the read
    }
    else if (address >= 0x8000 && address <= 0x9FFF)
    {
        dataRead = ppu.read(address);
    }
    else if (address >= 0xA000 && address <= 0xBFFF)
    {
        dataRead = internalRAM[address];
    }
    else if (address >= 0xC000 && address <= 0xDFFF)
    {
        dataRead = wram[address & 0x1FFF];
    }
    else if (address >= 0xE000 && address <= 0xFDFF) // (ECHO RAM)
    {
        dataRead = wram[address & 0x1DFF];
    }
    else if (address >= 0xFE00 && address <= 0xFE9F)
    {
        dataRead = ppu.read(address);
    }
    else if (address >= 0xFEA0 && address <= 0xFEFF)
    {
        dataRead = internalRAM[address];
    }
    else if (address >= 0xFF00 && address <= 0xFF7F)// IO Registers
    {
        switch (address)
        {
        case 0xFF00:
            return 0x0F; // TEMP
            break;
        case 0xFF01:
            dataRead = SB_register;
            break;
        case 0xFF02:
            dataRead = SC_register;
            break;
        case 0xFF04:
            dataRead = timer.read(address);
            break;
        case 0xFF05:
            dataRead = timer.read(address);
            break;
        case 0xFF06:
            dataRead = timer.read(address);
            break;
        case 0xFF07:
            dataRead = timer.read(address);
            break;
        case 0xFF0F:
            dataRead = 0xE0 | IF.reg;
            break;
        case 0xFF40:
            dataRead = ppu.read(address);
            break;
        case 0xFF41:
            dataRead = ppu.read(address);
            break;
        case 0xFF42:
            dataRead = ppu.read(address);
            break;
        case 0xFF43:
            dataRead = ppu.read(address);
            break;
        case 0xFF44:
            dataRead = ppu.read(address);
            //dataRead = 0x90; // Placeholder
            break;
        case 0xFF45:
            dataRead = ppu.read(address);
            break;
        case 0xFF47:
            dataRead = ppu.read(address);
            break;
        case 0xFF50:
            dataRead = bootROMMappedRegister;
            break;
        default:
            dataRead = internalRAM[address];
            //printf("Reading 0x%02X at $%04X\n", dataRead, address);
            break;
        }
    }
    else if (address >= 0xFF80 && address <= 0xFFFE)
    {
        dataRead = hram[address & 0x007F];
    }
    else if (address == 0xFFFF)
    {
        dataRead = 0xE0 | IE.reg;
    }
    /*else
    {
        __debugbreak();
    }*/

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
    if (address < 0x100 && ((bootROMMappedRegister & 0x01) == 0))
    {
        // BootROM is mapped in the first 256 bytes of address space so no writes allowed
    }
    else if (gamePak->write(address, data))
    {
        // Let the cartridge handle the write
    }
    else if (address >= 0x8000 && address <= 0x9FFF)
    {
        ppu.write(address, data);
    }
    else if (address >= 0xA000 && address <= 0xBFFF)
    {
        internalRAM[address] = data;
    }
    else if (address >= 0xC000 && address <= 0xDFFF)
    {
        wram[address & 0x1FFF] = data;
    }
    else if (address >= 0xE000 && address <= 0xFDFF) // (ECHO RAM)
    {
        wram[address & 0x1DFF] = data;
    }
    else if (address >= 0xFE00 && address <= 0xFE9F)
    {
        ppu.write(address, data);
    }
    else if (address >= 0xFEA0 && address <= 0xFEFF)
    {
        internalRAM[address] = data;
    }
    else if (address >= 0xFF00 && address <= 0xFF7F)// IO Registers
    {
        switch (address)
        {
        case 0xFF01:
            SB_register = data;
            break;
        case 0xFF02:
            if (data == 0x81)
                printf("%c", /*internalRAM[0xFF01]*/ SB_register);

            SC_register = data;
            break;
        case 0xFF04:
            timer.write(address, data);
            break;
        case 0xFF05:
            timer.write(address, data);
            break;
        case 0xFF06:
            timer.write(address, data);
            break;
        case 0xFF07:
            timer.write(address, data);
            break;
        case 0xFF0F:
            IF.reg = data;
            break;
        case 0xFF40:
            ppu.write(address, data);
            break;
        case 0xFF41:
            ppu.write(address, data);
            break;
        case 0xFF42:
            ppu.write(address, data);
            break;
        case 0xFF43:
            ppu.write(address, data);
            break;
        case 0xFF45:
            ppu.write(address, data);
            break;
        case 0xFF47:
            ppu.write(address, data);
            break;
        case 0xFF50:
            bootROMMappedRegister = data;
            break;
        default:
            internalRAM[address] = data;
            //printf("Writting 0x%02X to $%04X\n", data, address);
            break;
        }
    }
    else if (address >= 0xFF80 && address <= 0xFFFE)
    {
        hram[address & 0x007F] = data;
    }
    else if (address == 0xFFFF)
    {
        IE.reg = data;
    }
    /*else
    {
        __debugbreak();
    }*/
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
    ppu.reset();

    // TEMP for tests
    //skipBootROM();
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

    ppu.clock();
    timer.clock();

    if (isHaltMode)
    {
        if (checkPendingInterrupts())
        {
            isHaltMode = false;

            printf("HALT mode leaved, IME is %d\n", IME);

            //if (IME)
            //{
            //    //for (int i = 0; i < 5; i++)
            //        cpu.interruptServiceRoutine();
            //}
        }
    }
    else
    {
        cpu.clock();
    }

    systemCyclesElapsed++;
}

auto gb::GBConsole::step(u64 numberCycles) -> void
{
    for (size_t i = 0; i < numberCycles; i++)
        clock();
}

auto gb::GBConsole::requestInterrupt(InterruptType type) -> void
{
    switch (type)
    {
    case InterruptType::VBlank:
        IF.VBlank = 1;
        break;
    case InterruptType::STAT:
        IF.LCD_STAT = 1;
        break;
    case InterruptType::Timer:
        IF.Timer = 1;
        break;
    case InterruptType::Serial:
        IF.Serial = 1;
        break;
    case InterruptType::Joypad:
        IF.Joypad = 1;
        break;
    }
}

auto gb::GBConsole::getInterruptState(InterruptType type) -> u8
{
    switch (type)
    {
    case InterruptType::VBlank:
        return IF.VBlank;
        break;
    case InterruptType::STAT:
        return IF.LCD_STAT;
        break;
    case InterruptType::Timer:
        return IF.Timer;
        break;
    case InterruptType::Serial:
        return IF.Serial;
        break;
    case InterruptType::Joypad:
        return IF.Joypad;
        break;
    }
}

auto gb::GBConsole::getGameTitleFromHeader() -> std::string
{
    /*constexpr u16 startingOffset = 0x134;
    constexpr u16 endingOffset = 0x0143;
    constexpr u8 numBytes = endingOffset - startingOffset + 1;
    char title[numBytes] = { 0 };*/

    return std::string(reinterpret_cast<const char*>(gamePak->getHeaderInfo().title));
}

auto gb::GBConsole::skipBootROM() -> void
{
    //cpu.regs.PC = 0x0100;
    cpu.setRegisterValuesPostBootROM();
    timer.setDIVtoSkippedBootromValue();
}
