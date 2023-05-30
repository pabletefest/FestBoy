#include "ppu.h"
#include "gb.h"

namespace gb
{
    // "$8000 and $8800 addressing modes to access BG and Window Tile Data"
    static constexpr u16 vramAddressingMode[2][2] = { { 0x8000, 0x8800 }, { 0x9000, 0x8800 } };
}

gb::PPU::PPU(GBConsole* device)
    : system(device), LCDControl({}), LCDStatus({})
{
}

auto gb::PPU::read(u16 address) -> u8
{
    u8 dataRead = 0x00;

    if (address >= 0x8000 && address <= 0x9FFF)
    {
        /*if (LCDStatus.ModeFlag == 3)
            return 0xFF;*/

        dataRead = VRAM[address & 0x1FFF];
    }
    else if (address >= 0xFE00 && address <= 0xFE9F)
    {
        /*if (LCDStatus.ModeFlag == 3 || LCDStatus.ModeFlag == 2)
            return 0xFF;*/

        dataRead = OAM[address & 0x9F];
    }
    else
    {
        switch (address)
        {
        case 0xFF40:
            dataRead = LCDControl.reg;
            break;
        case 0xFF41:
            dataRead = LCDStatus.reg | 0x10; // No bit 7 so always read as a 1
            break;
        case 0xFF44:
            if (!LCDControl.LCDenable)
                LY = 0x00;

            dataRead = LY;
            break;
        case 0xFF45:
            dataRead = LYC;
            break;
        case 0xFF47:
            dataRead = bgPaletteData;
            break;
        }
    }

    return dataRead;
}

auto gb::PPU::write(u16 address, u8 data) -> void
{
    if (address >= 0x8000 && address <= 0x9FFF)
    {
        /*if (LCDStatus.ModeFlag == 3)
            return;*/

        VRAM[address & 0x1FFF] = data;
    }
    else if (address >= 0xFE00 && address <= 0xFE9F)
    {
        /*if (LCDStatus.ModeFlag == 3 || LCDStatus.ModeFlag == 2)
            return;*/

        OAM[address & 0x9F] = data;
    }
    else
    {
        switch (address)
        {
        case 0xFF40:
            LCDControl.reg = data;
            break;
        case 0xFF41:
            LCDStatus.reg |= (data & 0x71); // Only bits 6, 5, 4, and 3 are writable
            break;
        case 0xFF45:
            LYC = data;
            break;
        case 0xFF47:
            bgPaletteData = data;
            break;
        }
    }
}

auto gb::PPU::reset() -> void
{
}

auto gb::PPU::clock() -> void
{
    if (LYC == LY)
        LCDStatus.LYCLY_Flag = 1;

    // STAT Interrupt only triggered in rising edge, that is from low 0 to high 1
    if (!system->getInterruptState(gb::GBConsole::InterruptType::STAT) && LCDControl.LCDenable)
    {
        if ((LCDStatus.LYCLY_Flag && LCDStatus.LYCLYSTATIntrSrc) ||
            (LCDStatus.ModeFlag == 0 && LCDStatus.Mode0STATIntrSrc) ||
            (LCDStatus.ModeFlag == 2 && LCDStatus.Mode2STATIntrSrc) ||
            (LCDStatus.ModeFlag == 1 && LCDStatus.Mode1STATIntrSrc))
        {
            system->requestInterrupt(gb::GBConsole::InterruptType::STAT);
        }
    }

    if (LY >= 0 && LY <= 143)
    {
        // Mode 2 (OAM search)
        if (currentDot >= 0 && currentDot <= 79)
        {
            if (currentDot == 0)
                LCDStatus.ModeFlag = 2;

            lastMode3Dot = 174 + 80 - 1;// Min number of dots is 168-174 according to different sources (174 placeholder for now)
        }

        // Mode 3 (Rendering picture)
        if (currentDot >= 80 && currentDot <= lastMode3Dot)
        {
            if (currentDot == 80)
                LCDStatus.ModeFlag = 3;
        }

        // Mode 0 (HBlank period)
        if ((currentDot >= (lastMode3Dot + 1)) && (currentDot <= (remainingDots - 1)))
        {
            if (currentDot == (lastMode3Dot + 1))
                LCDStatus.ModeFlag = 0;
        }
    }
    else
    {
        // Mode 1 (VBlank period)
        if (LY == 144)
        {
            LCDStatus.ModeFlag = 1;
            system->requestInterrupt(gb::GBConsole::InterruptType::VBlank);
        }
    }

    currentDot++;
    remainingDots--;

    if (currentDot == 456)
    {
        currentDot = 0;
        LY++;

        if (LY == 154)
        {
            LY = 0;
        }
    }
}
