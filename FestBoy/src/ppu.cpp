/*
 * Copyright (C) 2023 pabletefest
 *
 * Licensed under GPLv3 or any later version.
 * Refer to the included LICENSE file.
 */

#include "ppu.h"
#include "gb.h"

namespace gb
{
    // "$8800 (LCD Control bit 4 is 0) and $8000 (LCD Control bit 4 is 1) addressing modes to access BG and Window Tile Data"
    static constexpr u16 vramAddressingMode[2][2] = { { 0x9000, 0x8800 }, { 0x8000, 0x8800 } };
    
    // When LCD Control bit 6 and/or 3 are set, tilemap base address is 0x9C00, 0x9800 otherwise.
    static constexpr u16 tileMapAddress[2] = { 0x9800, 0x9C00 };

    static constexpr PPU::Pixel greenShadesRGBPalette[4] = { { 155, 188, 15 }, { 139, 172, 15 }, { 48, 98, 48 }, { 15, 56, 15 } };
    static constexpr PPU::Pixel greyShadesRGBPalette[4] = { { 255, 255, 255 }, { 169, 169, 169 }, { 84, 84, 84 }, { 0, 0, 0 } };

    static constexpr u8 TILES_PER_LINE = 20;
    static constexpr u8 NUMBER_OF_TILE_LINES = 18;
    static constexpr u8 PIXELS_PER_LINE = 160;
    static constexpr u8 NUMBER_OF_LINES = 144;
}

gb::PPU::PPU(GBConsole* device)
    : system(device), LCDControl({}), LCDStatus({})
{
    std::memset(VRAM.data(), 0x00, VRAM.size());
    std::memset(OAM.data(), 0x00, OAM.size());
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
        case 0xFF42:
            dataRead = SCY;
            break;
        case 0xFF43:
            //SCX = dataRead;
            dataRead = SCX;
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
            LCDStatus.unused = 1;
            break;
        case 0xFF42:
            SCY = data;
            break;
        case 0xFF43:
            SCX = data;
            break;
        case 0xFF45:
            LYC = data;
            if (LYC == LY)
                LCDStatus.LYCLY_Flag = 1;
            else
                LCDStatus.LYCLY_Flag = 0;
            checkAndRaiseStatInterrupts();
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
    if (!LCDControl.LCDenable)
        return;

    //checkAndRaiseStatInterrupts();
   
    if (LY >= 0 && LY <= 143)
    {
        if (currentDot == 0)
        {
            if (LYC == LY)
                LCDStatus.LYCLY_Flag = 1;
            else
                LCDStatus.LYCLY_Flag = 0;

            checkAndRaiseStatInterrupts();
        }

        // Mode 2 (OAM search)
        if (currentDot >= 0 && currentDot <= 79)
        {
            if (currentDot == 0)
            {
                LCDStatus.ModeFlag = 2;
                lastMode3Dot = 172 + 80 - 1;// Min number of dots is 168-174 according to different sources (172 placeholder for now)
            }

            //checkAndRaiseStatInterrupts();
        }

        // Mode 3 (Rendering picture)
        if (currentDot >= 80 && currentDot <= lastMode3Dot)
        {
            if (currentDot == 80)
            {
                LCDStatus.ModeFlag = 3;
                //checkAndRaiseStatInterrupts();
            }

            // Render the line in the last dot before HBlank (scanline renderer)
            if (currentDot == lastMode3Dot)
            {
                if (LCDControl.BGWindEnablePriority)
                {
                    renderBackground();
                    renderWindow();
                }

                if (LCDControl.OBJenable)
                    renderSprites();
            }
        }

        // Mode 0 (HBlank period)
        //if ((currentDot >= (lastMode3Dot + 1)) && (currentDot <= (remainingDots - 1)))
        if ((currentDot >= (lastMode3Dot + 1)) && (currentDot <= (totalDotsPerScanline - 1)))
        {
            if (currentDot == (lastMode3Dot + 1))
            {
                LCDStatus.ModeFlag = 0;
                //checkAndRaiseStatInterrupts();
            }
        }
    }
    else
    {
        // Mode 1 (VBlank period)
        if (LY == 144 && currentDot == 0)
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
        remainingDots = 456;
        LY++;

        if (LY == 154)
        {
            LY = 0;
            frameCompleted = true;
        }
    }
}

auto gb::PPU::checkAndRaiseStatInterrupts() -> void
{
    /*if (!LCDControl.LCDenable)
        return;*/

    /*if (LYC == LY)
        LCDStatus.LYCLY_Flag = 1;
    else
        LCDStatus.LYCLY_Flag = 0;*/

    // STAT Interrupt only triggered in rising edge, that is from low 0 to high 1
    if (!system->getInterruptState(gb::GBConsole::InterruptType::STAT))
    {
        if ((LCDStatus.LYCLY_Flag && LCDStatus.LYCLYSTATIntrSrc) ||
            (LCDStatus.ModeFlag == 0 && LCDStatus.Mode0STATIntrSrc) ||
            (LCDStatus.ModeFlag == 2 && LCDStatus.Mode2STATIntrSrc) ||
            (LCDStatus.ModeFlag == 1 && LCDStatus.Mode1STATIntrSrc))
        {
            system->requestInterrupt(gb::GBConsole::InterruptType::STAT);

            printf("STAT Interrupt ocurred - IF: %d, LY: %d, LYC: %d\n", system->IF.reg, LY, LYC);
        }
    }
}

auto gb::PPU::renderBackground() -> void
{
    u8 tileLine = ((LY + SCY) / 8) % 32;
    u8 tileY = (LY + SCY) % 8;
    const u16* addressingMode = vramAddressingMode[LCDControl.BGWindTileDataArea];
    u16 bgTileMapAddress = tileMapAddress[LCDControl.BGtileMapArea] + (tileLine * 32)/* + ((SCX >> 3))*/; // Or (SCX / 8)

    for (int tileIndex = 0; tileIndex < TILES_PER_LINE; tileIndex++)
    {
        u8 tileId = read(bgTileMapAddress);
        //u16 tileDataAddress = (tileId < 128) ? *addressingMode : *(addressingMode + 1);
        u16 tileDataAddress = *addressingMode;

        if (LCDControl.BGWindTileDataArea == 0)
            tileDataAddress += static_cast<s8>(tileId) * 16;
        else
            tileDataAddress += tileId * 16;

        //tileDataAddress += (LCDControl.BGWindTileDataArea) ? tileId : static_cast<s8>(tileId);

        tileDataAddress += /*(tileId * 16) +*/ (tileY * 2);
        u8 lowByteTileData = read(tileDataAddress);
        u8 highByteTileData = read(tileDataAddress + 1);

        for (int pixelIndex = 0; pixelIndex < 8; pixelIndex++)
        {
            u8 lowBit = (lowByteTileData >> (7 - pixelIndex)) & 1;
            u8 highBit = (highByteTileData >> (7 - pixelIndex)) & 1;
            u8 paletteColorIndex = ((highBit << 1) | lowBit) & 0b11;
            u8 colorPixel = (bgPaletteData >> (paletteColorIndex * 2)) & 0b11;

            // (tileLine * 8 + tileY) * PIXELS_PER_LINE == LY * PIXELS_PER_LINE
            std::size_t bufferIndex = (LY * PIXELS_PER_LINE) + (tileIndex * 8 + pixelIndex);
            PPU::Pixel paletteColor = (LCDControl.BGWindEnablePriority) ? greenShadesRGBPalette[colorPixel & 0b11] : greenShadesRGBPalette[0];
            pixelsBuffer[bufferIndex] = paletteColor;

            //printf("Writting pixel %d of tile %d. Line: %d - Dot: %d\n", pixelIndex, tileIndex, LY, (tileIndex * 8) + pixelIndex);
        }

        bgTileMapAddress++;
        //bgTileMapAddress %= bgTileMapAddsress + 32;
    }

    //u8 pixelsShifted = SCX & 0b111;

    //// We shift the buffer by fine X pixels and overwrite that number of pixels of the next tile in the tile map
    //if (pixelsShifted > 0)
    //{
    //    std::shift_left(pixelsBuffer.begin(), pixelsBuffer.end(), pixelsShifted);

    //    u8 tileId = read(bgTileMapAddress);
    //    u16 tileDataAddress = (tileId < 128) ? *addressingMode : *(addressingMode + 1);
    //    tileDataAddress += (tileId * 16) + (tileY * 2);
    //    u8 lowByteTileData = read(tileDataAddress);
    //    u8 highByteTileData = read(tileDataAddress + 1);

    //    for (int pixelIndex = 0; pixelIndex < 8; pixelIndex++)
    //    {
    //        u8 lowBit = (lowByteTileData >> (7 - pixelIndex)) & 1;
    //        u8 highBit = (highByteTileData >> (7 - pixelIndex)) & 1;
    //        u8 paletteColorIndex = ((highBit << 1) | lowBit) & 0b11;
    //        u8 colorPixel = (bgPaletteData >> (paletteColorIndex * 2)) & 0b11;

    //        std::size_t bufferIndex = (LY * PIXELS_PER_LINE) + (19 * 8 + pixelIndex);
    //        PPU::Pixel paletteColor = (LCDControl.BGWindEnablePriority) ? greenShadesRGBPalette[colorPixel & 0b11] : greenShadesRGBPalette[0];
    //        pixelsBuffer[bufferIndex] = paletteColor;
    //    }
    //}
}

auto gb::PPU::renderWindow() -> void
{
}

auto gb::PPU::renderSprites() -> void
{
}
