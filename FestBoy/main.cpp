﻿/*
 * Copyright (C) 2023 pabletefest
 *
 * Licensed under GPLv3 or any later version.
 * Refer to the included LICENSE file.
 */

#pragma once
//#include "instructions.h"
#include "gb.h"
#include "cpu_sm83.h"
#include "game_pack.h"

#include <iostream>
#include <SDL.h>

int main(int argc, char* argv[])
{
    //std::cout << "Hello CMake." << std::endl;

    gb::GBConsole emulator;

    //Ref<gb::GamePak> cartridge = std::make_shared<gb::GamePak>("tests/blargg_tests/cpu_instrs/individual/01-special.gb");
    //Ref<gb::GamePak> cartridge = std::make_shared<gb::GamePak>("tests/blargg_tests/cpu_instrs/individual/02-interrupts.gb");
    //Ref<gb::GamePak> cartridge = std::make_shared<gb::GamePak>("tests/blargg_tests/cpu_instrs/individual/03-op sp,hl.gb");
    //Ref<gb::GamePak> cartridge = std::make_shared<gb::GamePak>("tests/blargg_tests/cpu_instrs/individual/04-op r,imm.gb");
    //Ref<gb::GamePak> cartridge = std::make_shared<gb::GamePak>("tests/blargg_tests/cpu_instrs/individual/05-op rp.gb");
    Ref<gb::GamePak> cartridge = std::make_shared<gb::GamePak>("tests/blargg_tests/cpu_instrs/individual/06-ld r,r.gb");
    //Ref<gb::GamePak> cartridge = std::make_shared<gb::GamePak>("tests/blargg_tests/cpu_instrs/individual/07-jr,jp,call,ret,rst.gb");
    //Ref<gb::GamePak> cartridge = std::make_shared<gb::GamePak>("tests/blargg_tests/cpu_instrs/individual/08-misc instrs.gb");
    //Ref<gb::GamePak> cartridge = std::make_shared<gb::GamePak>("tests/blargg_tests/cpu_instrs/individual/09-op r,r.gb");
    //Ref<gb::GamePak> cartridge = std::make_shared<gb::GamePak>("tests/blargg_tests/cpu_instrs/individual/10-bit ops.gb");
    //Ref<gb::GamePak> cartridge = std::make_shared<gb::GamePak>("tests/blargg_tests/cpu_instrs/individual/11-op a,(hl).gb");
    
    emulator.insertCartridge(cartridge);
    emulator.reset();

    //SDL_Window* window;
    //SDL_Renderer* renderer;

    //// Initialize SDL.
    //if (SDL_Init(SDL_INIT_VIDEO) < 0)
    //    return 1;

    //// Create the window where we will draw.
    //window = SDL_CreateWindow("SDL_RenderClear",
    //    SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
    //    512, 512,
    //    0);

    //// We must call SDL_CreateRenderer in order for draw calls to affect this window.
    //renderer = SDL_CreateRenderer(window, -1, 0);

    while (true)
    {
        emulator.clock();
        //// Select the color for drawing. It is set to random here.
        //SDL_SetRenderDrawColor(renderer, rand() % 256, rand() % 256, rand() % 256, 255);

        //// Clear the entire screen to our selected color.
        //SDL_RenderClear(renderer);

        //// Up until now everything was drawn behind the scenes.
        //// This will show the new, red contents of the window.
        //SDL_RenderPresent(renderer);

        //// Give us time to see the window.
        //SDL_Delay(1);
    }

    // Always be sure to clean up
    //SDL_Quit();

	return 0;
}
