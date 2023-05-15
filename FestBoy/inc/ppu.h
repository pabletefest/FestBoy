/*
 * Copyright (C) 2023 pabletefest
 *
 * Licensed under GPLv3 or any later version.
 * Refer to the included LICENSE file.
 */

#pragma once
#include "emu_typedefs.h"

class PPU
{
public:
    PPU();
    ~PPU() = default;
    
    auto read(u16 address) -> u8;
    auto write(u16 address, u8 data) -> void;

    auto reset() -> void;
    auto clock() -> void;

private:

};