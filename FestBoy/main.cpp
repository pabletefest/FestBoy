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
#include "ppu.h"

#include <iostream>
#include <SDL.h>

#define GB_PIXELS_HEIGHT 160
#define GB_PIXELS_WIDTH 144

static constexpr const char* EMU_TITLE = "FestBoy - A GameBoy emulator";

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
    //Ref<gb::GamePak> cartridge = std::make_shared<gb::GamePak>("tests/blargg_tests/cpu_instrs/cpu_instrs.gb");

    //Ref<gb::GamePak> cartridge = std::make_shared<gb::GamePak>("tests/dmg-acid2.gb");
    //Ref<gb::GamePak> cartridge = std::make_shared<gb::GamePak>("tests/gejmboj.gb");

    //Ref<gb::GamePak> cartridge = std::make_shared<gb::GamePak>("roms/Tetris.gb");
    Ref<gb::GamePak> cartridge = std::make_shared<gb::GamePak>("roms/Tetris V1.1.gb");
    //Ref<gb::GamePak> cartridge = std::make_shared<gb::GamePak>("roms/Dr. Mario.gb");
    //Ref<gb::GamePak> cartridge = std::make_shared<gb::GamePak>("roms/Super Mario Land.gb");
    //Ref<gb::GamePak> cartridge = std::make_shared<gb::GamePak>("roms/Donkey Kong.gb");
    
    emulator.insertCartridge(cartridge);
    emulator.reset();

    SDL_Window* window;
    SDL_Renderer* renderer;

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "Video Initialization Error: " << SDL_GetError() << "\n";
        return 1;
    }

    window = SDL_CreateWindow(std::string(EMU_TITLE + std::string(" (") + emulator.getGameTitleFromHeader() + std::string(")")).c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, GB_PIXELS_HEIGHT * 5, GB_PIXELS_WIDTH * 5, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

    if (!window)
    {
        std::cout << "Window Creation Error: " << SDL_GetError() << "\n";
        return 0;
    }

    //SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, SDL_LINEAR_FILTERING);
    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED );

    SDL_Texture* gameTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STREAMING, GB_PIXELS_HEIGHT, GB_PIXELS_WIDTH);

    bool isRunning = true;
    SDL_Event event;

    while (isRunning)
    {
        u64 startFrameTime = SDL_GetTicks64();    

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                isRunning = false;
            }
            else if (event.type == SDL_KEYDOWN)
            {
                SDL_Keycode keyCode = event.key.keysym.sym;

                switch (keyCode)
                {
                case SDLK_x: // A Button
                    //if (!(emulator.joypadRegister & 0x20))
                        emulator.controllerState.buttons &= ~0x1;
                    break;
                case SDLK_z: // B Button
                    //if (!(emulator.joypadRegister & 0x20))
                        emulator.controllerState.buttons &= ~0x2;
                    break;
                case SDLK_a: // SELECT Button
                    //if (!(emulator.joypadRegister & 0x20))
                        emulator.controllerState.buttons &= ~0x4;
                    break;
                case SDLK_s: // START Button
                    //if (!(emulator.joypadRegister & 0x20))
                        emulator.controllerState.buttons &= ~0x8;
                    break;
                case SDLK_UP: // D-PAD UP Button
                    //if (!(emulator.joypadRegister & 0x10))
                        emulator.controllerState.dpad &= ~0x4;
                    break;
                case SDLK_DOWN: // D-PAD DOWN Button
                    //if (!(emulator.joypadRegister & 0x10))
                        emulator.controllerState.dpad &= ~0x8;
                    break;
                case SDLK_LEFT: // D-PAD LEFT Button
                    //if (!(emulator.joypadRegister & 0x10))
                        emulator.controllerState.dpad &= ~0x2;
                    break;
                case SDLK_RIGHT: // D-PAD RIGHT Button
                    //if (!(emulator.joypadRegister & 0x10))
                        emulator.controllerState.dpad &= ~0x1;
                    break;
                }
            }
            else if (event.type == SDL_KEYUP)
            {
                SDL_Keycode keyCode = event.key.keysym.sym;

                switch (keyCode)
                {
                case SDLK_x: // A Button
                    //if (!(emulator.joypadRegister & 0x20))
                        emulator.controllerState.buttons |= 0x1;
                    break;
                case SDLK_z: // B Button
                    //if (!(emulator.joypadRegister & 0x20))
                        emulator.controllerState.buttons |= 0x2;
                    break;
                case SDLK_a: // SELECT Button
                    //if (!(emulator.joypadRegister & 0x20))
                        emulator.controllerState.buttons |= 0x4;
                    break;
                case SDLK_s: // START Button
                    //if (!(emulator.joypadRegister & 0x20))
                        emulator.controllerState.buttons |= 0x8;
                    break;
                case SDLK_UP: // D-PAD UP Button
                    //if (!(emulator.joypadRegister & 0x10))
                        emulator.controllerState.dpad |= 0x4;
                    break;
                case SDLK_DOWN: // D-PAD DOWN Button
                    //if (!(emulator.joypadRegister & 0x10))
                        emulator.controllerState.dpad |= 0x8;
                    break;
                case SDLK_LEFT: // D-PAD LEFT Button
                    //if (!(emulator.joypadRegister & 0x10))
                        emulator.controllerState.dpad |= 0x2;
                    break;
                case SDLK_RIGHT: // D-PAD RIGHT Button
                    //if (!(emulator.joypadRegister & 0x10))
                        emulator.controllerState.dpad |= 0x1;
                    break;
                }
            }
        }

        do
        {
            emulator.clock();
        } while (!emulator.getPPU().frameCompleted);

        emulator.getPPU().frameCompleted = false;

        SDL_UpdateTexture(gameTexture, nullptr, emulator.getPPU().getPixelsBufferData(), sizeof(gb::PPU::Pixel) * 160);

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, gameTexture, NULL, NULL);
        SDL_RenderPresent(renderer);

        //SDL_Delay(16);

        u64 endFrameTime = SDL_GetTicks64();
        int idleTime = 16 - (endFrameTime - startFrameTime); // No floating number so rounded to 16ms per frame
        if (idleTime < 0) idleTime = 0;
        //printf("Sleeping for %d secs\n", (int)idleTime);
        SDL_Delay(static_cast<u32>(idleTime)); // Sleep the remaining time of the frame after being calculated and drawn to the screen
    }

    // Always be sure to clean up
    SDL_DestroyTexture(gameTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

	return 0;
}
