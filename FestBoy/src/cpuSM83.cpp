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
        LD<REGISTER, IMMEDIATE, u16>(this, regs.BC, read16(regs.PC));
        regs.PC += 2;
        break;
    case 0x02:
        LD<ADDRESS_PTR, REGISTER, u16>(this, regs.BC, regs.A);
        break;
    case 0x06:
        LD<REGISTER, IMMEDIATE, u8>(this, regs.B, read8(regs.PC++));
        break;
    case 0x08:
        {
            u16 address = read16(regs.PC);
            LD<ADDRESS_PTR, REGISTER, u16>(this, address, regs.SP);
            regs.PC += 2;
        }
        break;
    case 0x0A:
        LD<REGISTER, IMMEDIATE, u8>(this, regs.A, read8(regs.BC));
        break;
    case 0x0E:
        LD<REGISTER, IMMEDIATE, u8>(this, regs.C, read8(regs.PC++));
        break;
    case 0x11:
        LD<REGISTER, IMMEDIATE, u16>(this, regs.DE, read16(regs.PC));
        regs.PC += 2;
        break;
    case 0x12:
        LD<ADDRESS_PTR, REGISTER, u16>(this, regs.DE, regs.A);
        break;
    case 0x16:
        LD<REGISTER, IMMEDIATE, u8>(this, regs.D, read8(regs.PC++));
        break;
    case 0x1A:
        LD<REGISTER, IMMEDIATE, u8>(this, regs.A, read8(regs.DE));
        break;
    case 0x1E:
        LD<REGISTER, IMMEDIATE, u8>(this, regs.E, read8(regs.PC++));
        break;
    case 0x21:
        LD<REGISTER, IMMEDIATE, u16>(this, regs.HL, read16(regs.PC));
        regs.PC += 2;
        break;
    case 0x22:
        LD<ADDRESS_PTR, REGISTER, u16>(this, regs.HL, regs.A);
        regs.HL++;
        break;
    case 0x26:
        LD<REGISTER, IMMEDIATE, u8>(this, regs.H, read8(regs.PC++));
        break;
    case 0x2A:
        LD<REGISTER, IMMEDIATE, u8>(this, regs.A, read8(regs.HL++));
        break;
    case 0x2E:
        LD<REGISTER, IMMEDIATE, u8>(this, regs.L, read8(regs.PC++));
        break;
    case 0x31:
        LD<REGISTER, IMMEDIATE, u16>(this, regs.HL, read16(regs.PC));
        regs.PC += 2;
        break;
    case 0x32:
        LD<ADDRESS_PTR, REGISTER, u16>(this, regs.HL, regs.A);
        regs.HL--;
        break;
    case 0x36:
        LD<ADDRESS_PTR, IMMEDIATE, u16>(this, regs.HL, read8(regs.PC++));
        break;
    case 0x3A:
        LD<REGISTER, IMMEDIATE, u8>(this, regs.A, read8(regs.BC--));
        break;
    case 0x3E:
        LD<REGISTER, IMMEDIATE, u8>(this, regs.A, read8(regs.PC++));
        break;
    case 0x40:
        LD<REGISTER, REGISTER, u8>(this, regs.B, regs.B);
        break;
    case 0x41:
        LD<REGISTER, REGISTER, u8>(this, regs.B, regs.C);
        break;
    case 0x42:
        LD<REGISTER, REGISTER, u8>(this, regs.B, regs.D);
        break;
    case 0x43:
        LD<REGISTER, REGISTER, u8>(this, regs.B, regs.E);
        break;
    case 0x44:
        LD<REGISTER, REGISTER, u8>(this, regs.B, regs.H);
        break;
    case 0x45:
        LD<REGISTER, REGISTER, u8>(this, regs.B, regs.L);
        break;
    case 0x46:
        LD<REGISTER, IMMEDIATE, u8>(this, regs.B, read8(regs.HL));
        break;
    case 0x47:
        LD<REGISTER, REGISTER, u8>(this, regs.B, regs.A);
        break;
    case 0x48:
        LD<REGISTER, REGISTER, u8>(this, regs.C, regs.B);
        break;
    case 0x49:
        LD<REGISTER, REGISTER, u8>(this, regs.C, regs.C);
        break;
    case 0x4A:
        LD<REGISTER, REGISTER, u8>(this, regs.C, regs.D);
        break;
    case 0x4B:
        LD<REGISTER, REGISTER, u8>(this, regs.C, regs.E);
        break;
    case 0x4C:
        LD<REGISTER, REGISTER, u8>(this, regs.C, regs.H);
        break;
    case 0x4D:
        LD<REGISTER, REGISTER, u8>(this, regs.C, regs.L);
        break;
    case 0x4E:
        LD<REGISTER, IMMEDIATE, u8>(this, regs.C, read8(regs.HL));
        break;
    case 0x4F:
        LD<REGISTER, REGISTER, u8>(this, regs.C, regs.A);
        break;
    case 0x50:
        LD<REGISTER, REGISTER, u8>(this, regs.D, regs.B);
        break;
    case 0x51:
        LD<REGISTER, REGISTER, u8>(this, regs.D, regs.C);
        break;
    case 0x52:
        LD<REGISTER, REGISTER, u8>(this, regs.D, regs.D);
        break;
    case 0x53:
        LD<REGISTER, REGISTER, u8>(this, regs.D, regs.E);
        break;
    case 0x54:
        LD<REGISTER, REGISTER, u8>(this, regs.D, regs.H);
        break;
    case 0x55:
        LD<REGISTER, REGISTER, u8>(this, regs.D, regs.L);
        break;
    case 0x56:
        LD<REGISTER, IMMEDIATE, u8>(this, regs.D, read8(regs.HL));
        break;
    case 0x57:
        LD<REGISTER, REGISTER, u8>(this, regs.D, regs.A);
        break;
    case 0x58:
        LD<REGISTER, REGISTER, u8>(this, regs.E, regs.B);
        break;
    case 0x59:
        LD<REGISTER, REGISTER, u8>(this, regs.E, regs.C);
        break;
    case 0x5A:
        LD<REGISTER, REGISTER, u8>(this, regs.E, regs.D);
        break;
    case 0x5B:
        LD<REGISTER, REGISTER, u8>(this, regs.E, regs.E);
        break;
    case 0x5C:
        LD<REGISTER, REGISTER, u8>(this, regs.E, regs.H);
        break;
    case 0x5D:
        LD<REGISTER, REGISTER, u8>(this, regs.E, regs.L);
        break;
    case 0x5E:
        LD<REGISTER, IMMEDIATE, u8>(this, regs.E, read8(regs.HL));
        break;
    case 0x5F:
        LD<REGISTER, REGISTER, u8>(this, regs.E, regs.A);
        break;
    case 0x60:
        LD<REGISTER, REGISTER, u8>(this, regs.H, regs.B);
        break;
    case 0x61:
        LD<REGISTER, REGISTER, u8>(this, regs.H, regs.C);
        break;
    case 0x62:
        LD<REGISTER, REGISTER, u8>(this, regs.H, regs.D);
        break;
    case 0x63:
        LD<REGISTER, REGISTER, u8>(this, regs.H, regs.E);
        break;
    case 0x64:
        LD<REGISTER, REGISTER, u8>(this, regs.H, regs.H);
        break;
    case 0x65:
        LD<REGISTER, REGISTER, u8>(this, regs.H, regs.L);
        break;
    case 0x66:
        LD<REGISTER, IMMEDIATE, u8>(this, regs.H, read8(regs.HL));
        break;
    case 0x67:
        LD<REGISTER, REGISTER, u8>(this, regs.H, regs.A);
        break;
    case 0x68:
        LD<REGISTER, REGISTER, u8>(this, regs.L, regs.B);
        break;
    case 0x69:
        LD<REGISTER, REGISTER, u8>(this, regs.L, regs.C);
        break;
    case 0x6A:
        LD<REGISTER, REGISTER, u8>(this, regs.L, regs.D);
        break;
    case 0x6B:
        LD<REGISTER, REGISTER, u8>(this, regs.L, regs.E);
        break;
    case 0x6C:
        LD<REGISTER, REGISTER, u8>(this, regs.L, regs.H);
        break;
    case 0x6D:
        LD<REGISTER, REGISTER, u8>(this, regs.L, regs.L);
        break;
    case 0x6E:
        LD<REGISTER, REGISTER, u8>(this, regs.L, read8(regs.HL));
        break;
    case 0x6F:
        LD<REGISTER, REGISTER, u8>(this, regs.L, regs.A);
        break;
    case 0x70:
        LD<ADDRESS_PTR, REGISTER, u16>(this, regs.HL, regs.B);
        break;
    case 0x71:
        LD<ADDRESS_PTR, REGISTER, u16>(this, regs.HL, regs.C);
        break;
    case 0x72:
        LD<ADDRESS_PTR, REGISTER, u16>(this, regs.HL, regs.D);
        break;
    case 0x73:
        LD<ADDRESS_PTR, REGISTER, u16>(this, regs.HL, regs.E);
        break;
    case 0x74:
        LD<ADDRESS_PTR, REGISTER, u16>(this, regs.HL, regs.H);
        break;
    case 0x75:
        LD<ADDRESS_PTR, REGISTER, u16>(this, regs.HL, regs.L);
        break;
    case 0x77:
        break;
    case 0x78:
        LD<ADDRESS_PTR, REGISTER, u16>(this, regs.HL, regs.A);
        break;
    case 0x79:
        LD<REGISTER, REGISTER, u8>(this, regs.L, regs.A);
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
        POP(this, regs.BC);
        break;
    case 0xC5:
        PUSH(this, regs.BC);
        break;
    case 0xCB:
        //u8 cbOpcode = read8(regs.PC++);
        decodeAndExecuteCBInstruction(read8(regs.PC++));
        break;
    case 0xD1:
        POP(this, regs.DE);
        break;
    case 0xD5:
        PUSH(this, regs.DE);
        break;
    case 0xE0:
        {
            u16 address = 0xFF00 + read8(regs.PC++);
            LD<ADDRESS_PTR, REGISTER, u16>(this, address, regs.A);
        }
        break;
    case 0xE1:
        POP(this, regs.HL);
        break;
    case 0xE2:
        {
            u16 address = 0xFF00 + regs.C;
            LD<ADDRESS_PTR, REGISTER, u16>(this, address, regs.A);
        }
        break;
    case 0xE5:
        PUSH(this, regs.HL);
        break;
    case 0xEA:
        {
            u16 address = read16(regs.PC);
            LD<ADDRESS_PTR, REGISTER, u16>(this, address, regs.A);
            regs.PC += 2;
        }
        break;
    case 0xF0:
        LD<REGISTER, IMMEDIATE, u8>(this, regs.A, read8(0xFF00 + read8(regs.PC++)));
        break;
    case 0xF1:
        POP(this, regs.AF);
        break;
    case 0xF2:
        LD<REGISTER, IMMEDIATE, u8>(this, regs.A, read8(0xFF00 + regs.C));
        break;
    case 0xF5:
        PUSH(this, regs.AF);
        break;
    case 0xF9:
        LD<REGISTER, REGISTER, u16>(this, regs.SP, regs.HL);
        break;
    case 0xFA:
        LD<REGISTER, IMMEDIATE, u8>(this, regs.A, read8(regs.PC));
        regs.PC += 2;
        break;
    default:
        printf("Opcode %02X not implemented", opcode);
        break;
    }
}

auto gb::SM83CPU::decodeAndExecuteCBInstruction(u8 cbOpcode) -> void
{
}
