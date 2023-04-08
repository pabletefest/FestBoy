#include "cpuSM83.h"
#include "gb.h"
#include "instructions.h"

gb::SM83CPU::SM83CPU(GBConsole* device)
    : system(device), regs({})
{
}

auto gb::SM83CPU::read8(const u16& address) -> u8
{
    return system->read8(address);
}

auto gb::SM83CPU::read16(const u16& address) -> u16
{
    return system->read16(address);
}

auto gb::SM83CPU::write8(const u16& address, const u8& data) -> void
{
    system->write8(address, data);
}

auto gb::SM83CPU::write16(const u16& address, const u16& data) -> void
{
    system->write16(address, data);
}

auto gb::SM83CPU::reset() -> void
{
}

auto gb::SM83CPU::clock() -> void
{
    if (instructionCycles == 0) // Time to fetch and execute next opcode
    {
        u8 opcode = read8(regs.PC++);
        decodeAndExecuteInstruction(opcode);
    }

    instructionCycles--;
    cpuCyclesElapsed++;
}

auto gb::SM83CPU::decodeAndExecuteInstruction(u8 opcode) -> void
{
    switch (opcode)
    {
    case 0x01:
        u16 value = read16(regs.PC);
        regs.PC += 2;
        LD<REGISTER, IMMEDIATE, u16>(this, regs.BC, value);
        break;
    case 0x02:
        break;
    case 0x06:
        break;
    case 0x08:
        break;
    case 0x0A:
        break;
    case 0x0E:
        break;
    case 0x11:
        u16 value = read16(regs.PC);
        regs.PC += 2;
        LD<REGISTER, IMMEDIATE, u16>(this, regs.DE, value);
        break;
    case 0x12:
        break;
    case 0x16:
        break;
    case 0x1A:
        break;
    case 0x1E:
        break;
    case 0x21:
        u16 value = read16(regs.PC);
        regs.PC += 2;
        LD<REGISTER, IMMEDIATE, u16>(this, regs.HL, value);
        break;
    case 0x22:
        break;
    case 0x26:
        break;
    case 0x2A:
        break;
    case 0x2E:
        break;
    case 0x31:
        u16 value = read16(regs.PC);
        regs.PC += 2;
        LD<REGISTER, IMMEDIATE, u16>(this, regs.HL, value);
        break;
    case 0x32:
        break;
    case 0x36:
        break;
    case 0x3A:
        break;
    case 0x3E:
        break;
    case 0x40:
        break;
    case 0x41:
        break;
    case 0x42:
        break;
    case 0x43:
        break;
    case 0x44:
        break;
    case 0x45:
        break;
    case 0x46:
        break;
    case 0x47:
        break;
    case 0x48:
        break;
    case 0x49:
        break;
    case 0x4A:
        break;
    case 0x4B:
        break;
    case 0x4C:
        break;
    case 0x4D:
        break;
    case 0x4E:
        break;
    case 0x4F:
        break;
    case 0x50:
        break;
    case 0x51:
        break;
    case 0x52:
        break;
    case 0x53:
        break;
    case 0x54:
        break;
    case 0x55:
        break;
    case 0x56:
        break;
    case 0x57:
        break;
    case 0x58:
        break;
    case 0x59:
        break;
    case 0x5A:
        break;
    case 0x5B:
        break;
    case 0x5C:
        break;
    case 0x5D:
        break;
    case 0x5E:
        break;
    case 0x5F:
        break;
    case 0x60:
        break;
    case 0x61:
        break;
    case 0x62:
        break;
    case 0x63:
        break;
    case 0x64:
        break;
    case 0x65:
        break;
    case 0x66:
        break;
    case 0x67:
        break;
    case 0x68:
        break;
    case 0x69:
        break;
    case 0x6A:
        break;
    case 0x6B:
        break;
    case 0x6C:
        break;
    case 0x6D:
        break;
    case 0x6E:
        break;
    case 0x6F:
        break;
    case 0x70:
        break;
    case 0x71:
        break;
    case 0x72:
        break;
    case 0x73:
        break;
    case 0x74:
        break;
    case 0x75:
        break;
    case 0x77:
        break;
    case 0x78:
        break;
    case 0x79:
        break;
    case 0x7A:
        break;
    case 0x7B:
        break;
    case 0x7C:
        break;
    case 0x7D:
        break;
    case 0x7E:
        break;
    case 0x7F:
        break;
    case 0xC1:
        break;
    case 0xC5:
        break;
    case 0xCB:
        u8 cbOpcode = read8(regs.PC++);
        decodeAndExecuteCBInstruction(cbOpcode);
        break;
    case 0xD1:
        break;
    case 0xD5:
        break;
    case 0xE0:
        break;
    case 0xE1:
        break;
    case 0xE2:
        break;
    case 0xE5:
        break;
    case 0xEA:
        break;
    case 0xF0:
        break;
    case 0xF1:
        break;
    case 0xF2:
        break;
    case 0xF5:
        break;
    case 0xF9:
        break;
    case 0xFA:
        break;
    default:
        printf("Opcode %02X not implemented", opcode);
        break;
    }
}

auto gb::SM83CPU::decodeAndExecuteCBInstruction(u8 cbOpcode) -> void
{
}
