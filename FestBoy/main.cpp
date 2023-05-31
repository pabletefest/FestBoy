/*
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
#include "opcodesTests.h"

#include <iostream>
#include <SDL.h>

int main(int argc, char* argv[])
{
#if 0
    int counter = 0;
    std::ifstream ifs1;
    ifs1.open("cpu_log.txt");

    std::ifstream ifs2;
    ifs2.open("tests/Gameboy-logs/Blargg2LYStubbed/EpicLog.txt");

    if (ifs1.is_open() && ifs2.is_open())
    {
        std::string line1;
        std::string line2;

        std::string prev1;
        std::string prev2;

        while (std::getline(ifs1, line1) && std::getline(ifs2, line2))
        {
            if (line1 != line2)
            {
                printf("Line diff at %d\n", counter);
                printf("Original lines: \n");
                printf("%s\n", prev2.c_str());
                printf("%s\n", line2.c_str());
                printf("Actual lines: \n");
                printf("%s\n", prev1.c_str());
                printf("%s\n", line1.c_str());
                printf("\n");
            }

            prev1 = line1;
            prev2 = line2;

            counter++;
        }

        printf("\nBlargg' test log and current emulator log matches and passes the test\n");
    }
#endif

    //std::cout << "Hello CMake." << std::endl;

    gb::GBConsole emulator;

    //testAllOpcodes(&emulator.getCPU());

    //Ref<gb::GamePak> cartridge = std::make_shared<gb::GamePak>("tests/blargg_tests/cpu_instrs/individual/01-special.gb");
    //Ref<gb::GamePak> cartridge = std::make_shared<gb::GamePak>("tests/blargg_tests/cpu_instrs/individual/02-interrupts.gb");
    //Ref<gb::GamePak> cartridge = std::make_shared<gb::GamePak>("tests/blargg_tests/cpu_instrs/individual/03-op sp,hl.gb");
    //Ref<gb::GamePak> cartridge = std::make_shared<gb::GamePak>("tests/blargg_tests/cpu_instrs/individual/04-op r,imm.gb");
    //Ref<gb::GamePak> cartridge = std::make_shared<gb::GamePak>("tests/blargg_tests/cpu_instrs/individual/05-op rp.gb");
    //Ref<gb::GamePak> cartridge = std::make_shared<gb::GamePak>("tests/blargg_tests/cpu_instrs/individual/06-ld r,r.gb");
    //Ref<gb::GamePak> cartridge = std::make_shared<gb::GamePak>("tests/blargg_tests/cpu_instrs/individual/07-jr,jp,call,ret,rst.gb");
    //Ref<gb::GamePak> cartridge = std::make_shared<gb::GamePak>("tests/blargg_tests/cpu_instrs/individual/08-misc instrs.gb");
    //Ref<gb::GamePak> cartridge = std::make_shared<gb::GamePak>("tests/blargg_tests/cpu_instrs/individual/09-op r,r.gb");
    //Ref<gb::GamePak> cartridge = std::make_shared<gb::GamePak>("tests/blargg_tests/cpu_instrs/individual/10-bit ops.gb");
    //Ref<gb::GamePak> cartridge = std::make_shared<gb::GamePak>("tests/blargg_tests/cpu_instrs/individual/11-op a,(hl).gb");
    Ref<gb::GamePak> cartridge = std::make_shared<gb::GamePak>("tests/blargg_tests/cpu_instrs/cpu_instrs.gb");

    //Ref<gb::GamePak> cartridge = std::make_shared<gb::GamePak>("tests/dmg-acid2.gb");
    
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
