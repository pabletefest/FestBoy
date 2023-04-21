#pragma once

#include "cpu_sm83.h"

#include <nlohmann/json.hpp>

#include <istream>
#include <cassert>
#include <array>

void testOpcodes(gb::SM83CPU* cpu)
{
    std::array<u8, 64 * 1024> testRAM;

    using json = nlohmann::json;

    json j = json::parse(std::ifstream("tests/SM83/v1/00.json"));
    
    std::string testNumber = j.at(0).at("name").get<std::string>().substr(3);
    u8 opcode = static_cast<u8>(std::stoul(j.at(0).at("name").get<std::string>().substr(0, 2), nullptr, 16));
    
    printf("Running test %02X %s.json\n", opcode, testNumber.c_str());

    // Setting up cpu initial state
    json initStateJson = j.at(0).at("initial");
    
    cpu->regs.PC = initStateJson.at("pc").get<u16>();
    cpu->regs.SP = initStateJson.at("sp").get<u16>();
    cpu->regs.A = initStateJson.at("a").get<u8>();
    cpu->regs.B = initStateJson.at("b").get<u8>();
    cpu->regs.C = initStateJson.at("c").get<u8>();
    cpu->regs.D = initStateJson.at("d").get<u8>();
    cpu->regs.E = initStateJson.at("e").get<u8>();
    cpu->regs.F = initStateJson.at("f").get<u8>();
    cpu->regs.H = initStateJson.at("h").get<u8>();
    cpu->regs.L = initStateJson.at("l").get<u8>();

    for (const auto& elem : initStateJson.at("ram"))
        testRAM[elem.at(0).get<u16>()] = elem.at(1).get<u8>();

    //cpu->write8(cpu->regs.PC, opcode);

    for (std::size_t cycle = 0; cycle < initStateJson.at("cycles").size() * 4; cycle++)
        cpu->clock();

    json finalStateJson = j.at(0).at("final");

    u16 finalPC = finalStateJson.at("pc").get<u16>();
    u16 finalSP = finalStateJson.at("sp").get<u16>();
    u16 finalA = finalStateJson.at("a").get<u8>();
    u16 finalB = finalStateJson.at("b").get<u8>();
    u16 finalC = finalStateJson.at("c").get<u8>();
    u16 finalD = finalStateJson.at("d").get<u8>();
    u16 finalE = finalStateJson.at("e").get<u8>();
    u16 finalF = finalStateJson.at("f").get<u8>();
    u16 finalH = finalStateJson.at("h").get<u8>();
    u16 finalL = finalStateJson.at("l").get<u8>();

    printf("\n-------------------------------------------------------------------------------------------------------------\n");

    char expectedLog[256];
    char actualLog[256];

    printf("\nRAM contents\n");

    printf("Expected: \n");


    printf("Actual: \n");

    printf("\nCPU contents\n");

    printf("Expected: \n");
    sprintf(expectedLog, "A: %02X F: %02X B: %02X C: %02X D: %02X E: %02X H: %02X L: %02X SP: %04X PC: 00:%04X (%02X %02X %02X %02X)\n", finalA, finalF, finalB, finalC, finalD, finalE, finalH, finalL, finalSP, finalPC - 1, opcode, finalPC, finalPC + 1, finalPC + 2);
    printf(expectedLog);

    printf("Actual: \n");
    sprintf(actualLog, "A: %02X F: %02X B: %02X C: %02X D: %02X E: %02X H: %02X L: %02X SP: %04X PC: 00:%04X (%02X %02X %02X %02X)\n", cpu->regs.A, cpu->regs.F, cpu->regs.B, cpu->regs.C, cpu->regs.D, cpu->regs.E, cpu->regs.H, cpu->regs.L, cpu->regs.SP, cpu->regs.PC - 1, opcode, testRAM[cpu->regs.PC], testRAM[cpu->regs.PC + 1], testRAM[cpu->regs.PC + 2]);
    printf(actualLog);

    if (std::string(expectedLog) == std::string(actualLog))
    { 
        printf("\nTest %02X %s passed succesfully!\n", opcode, testNumber.c_str());
    }
    else
    {
        printf("\nTest %02X %s failed, check implementation!\n", opcode, testNumber.c_str());
        assert(false);
    }

    printf("\n-------------------------------------------------------------------------------------------------------------\n");
}