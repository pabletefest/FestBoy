#include "ppu.h"

PPU::PPU()
{
}

auto PPU::read(u16 address) -> u8
{
    return u8();
}

auto PPU::write(u16 address, u8 data) -> void
{
}

auto PPU::reset() -> void
{
}

auto PPU::clock() -> void
{
    if (LY >= 0 && LY <= 143)
    {
        // Mode 2 (OAM search)
        if (currentDot >= 0 && currentDot <= 79)
        {
            lastMode3Dot = 168 + 80 - 1;// Min number of dots is 168 (placeholder)
        }

        // Mode 3 (Rendering picture)
        if (currentDot >= 80 && currentDot <= lastMode3Dot)
        {

        }

        // Mode 0 (HBlank period)
        if ((currentDot >= (lastMode3Dot + 1)) && (currentDot <= (remainingDots - 1)))
        {

        }
    }
    else
    {
        // Mode 1 (VBlank period)

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
