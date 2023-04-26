#pragma once

#include "cpu_sm83.h"

#include <nlohmann/json.hpp>

#include <fstream>
#include <cassert>
#include <array>
#include <iostream>

auto testOpcode(gb::SM83CPU* cpu, u8 opcodeTest, bool extendedOpcode) -> void;

auto testAllOpcodes(gb::SM83CPU* cpu) -> void
{
    for (int test = 0xF0; test < 256; test++)
    {
        if (test == 0xD3 || test == 0xDB || test == 0xDD || test == 0xE3 || test == 0xE4
            || test == 0xEB || test == 0xEC || test == 0xED || test == 0xF4 || test == 0xFC
            || test == 0xFD || test == 0x10 || test == 0x76)
            continue;

        if (test == 0xCB)
        {
            for (int extendedTest = 0; extendedTest < 256; extendedTest++)
                testOpcode(cpu, extendedTest, true);
        }
        else
        {
            testOpcode(cpu, test, false);
        }
    }

    printf("\nALL CPU OPCODES PASSED. CONGRATULATIONS! PRETTY ROBUST CPU IMPLEMENTATION\n");
    assert(false);
}

auto testOpcode(gb::SM83CPU* cpu, u8 opcodeTest, bool extendedOpcode) -> void
{
    std::array<u8, 64 * 1024> testRAM;
    std::memset(testRAM.data(), 0x00, sizeof(testRAM));
    cpu->debugRAM = testRAM.data();

    using json = nlohmann::json;

    char testPath[64];

    if (extendedOpcode)
        sprintf_s(testPath, "tests/SM83/v1/CB %02X.json", opcodeTest);
    else
        sprintf_s(testPath, "tests/SM83/v1/%02X.json", opcodeTest);

    json j = json::parse(std::ifstream(testPath));

    for (std::size_t test = 0; test < j.size(); test++)
    {
        std::string testNumber = j.at(test).at("name").get<std::string>().substr(3);
        u8 opcode = static_cast<u8>(std::stoul(j.at(test).at("name").get<std::string>().substr(0, 2), nullptr, 16));

        printf("\n***** RUNNING TEST %02X %s.json *****\n", opcode, testNumber.c_str());

        // Setting up cpu initial state
        json initStateJson = j.at(test).at("initial");

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

        printf("\nInitial RAM contents\n");
        for (const auto& elem : initStateJson.at("ram"))
            printf("[%d, %d]\n", elem.at(0).get<u16>(), testRAM[elem.at(0).get<u16>()]);

        printf("\nCPU initial state\n");
        printf("A: %d F: %d B: %d C: %d D: %d E: %d H: %d L: %d SP: %d PC: 00:%d\n", cpu->regs.A, cpu->regs.F, cpu->regs.B, cpu->regs.C, cpu->regs.D, cpu->regs.E, cpu->regs.H, cpu->regs.L, cpu->regs.SP, cpu->regs.PC);

        //cpu->write8(cpu->regs.PC, opcode);

        std::size_t m_cycles = j.at(test).at("cycles").size();

        for (std::size_t cycle = 0; cycle < m_cycles * 4; cycle++)
            cpu->clock();

        json finalStateJson = j.at(test).at("final");

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

        printf("\n-- RAM contents --\n");

        printf("-> Expected: \n");
        std::string expectedRAM;

        for (const auto& elem : finalStateJson.at("ram"))
            expectedRAM += "[" + std::to_string(elem.at(0).get<u16>()) + ", " + std::to_string(elem.at(1).get<u8>()) + "]\n";

        printf(expectedRAM.c_str());

        printf("-> Actual: \n");
        std::string actualRAM;

        for (const auto& elem : finalStateJson.at("ram"))
            actualRAM += "[" + std::to_string(elem.at(0).get<u16>()) + ", " + std::to_string(testRAM[elem.at(0).get<u16>()]) + "]\n";

        printf(actualRAM.c_str());

        printf("\n-- CPU contents --\n");

        printf("-> Expected: \n");
        sprintf_s(expectedLog, "A: %d F: %d B: %d C: %d D: %d E: %d H: %d L: %d SP: %d PC: 00:%d\n", finalA, finalF, finalB, finalC, finalD, finalE, finalH, finalL, finalSP, finalPC - 1);
        printf(expectedLog);

        printf("-> Actual: \n");
        sprintf_s(actualLog, "A: %d F: %d B: %d C: %d D: %d E: %d H: %d L: %d SP: %d PC: 00:%d\n", cpu->regs.A, cpu->regs.F, cpu->regs.B, cpu->regs.C, cpu->regs.D, cpu->regs.E, cpu->regs.H, cpu->regs.L, cpu->regs.SP, cpu->regs.PC - 1);
        printf(actualLog);

        if ((std::string(expectedLog) == std::string(actualLog)) && (expectedRAM == actualRAM))
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

    printf("\nALL TESTS FOR OPCODE %02X IN FILE %02X.json PASSED!\n", opcodeTest, opcodeTest);
}