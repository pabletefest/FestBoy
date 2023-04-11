#pragma once
//#include "cpuSM83.h"
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
    cpuT_CyclesElapsed++;
    cpuM_CyclesElapsed = cpuT_CyclesElapsed / 4;
}

auto gb::SM83CPU::decodeAndExecuteInstruction(u8 opcode) -> void
{
    switch (opcode)
    {
    case 0x00:
        NOP();
        break;
    case 0x01:
        LD<REGISTER, IMMEDIATE, u16>(this, regs.BC, read16(regs.PC));
        regs.PC += 2;
        break;
    case 0x02:
        LD<ADDRESS_PTR, REGISTER, u16>(this, regs.BC, regs.A);
        break;
    case 0x03:
        INC<REGISTER, u16>(this, regs.BC);
        break;
    case 0x04:
        INC<REGISTER, u8>(this, regs.B);
        break;
    case 0x05:
        DEC<REGISTER, u8>(this, regs.B);
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
    case 0x09:
        ADD_HLrr(this, regs.BC);
        break;
    case 0x0A:
        LD<REGISTER, IMMEDIATE, u8>(this, regs.A, read8(regs.BC));
        break;
    case 0x0B:
        DEC<REGISTER, u16>(this, regs.BC);
        break;
    case 0x0C:
        INC<REGISTER, u8>(this, regs.C);
        break;
    case 0x0D:
        DEC<REGISTER, u8>(this, regs.C);
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
    case 0x13:
        INC<REGISTER, u16>(this, regs.DE);
        break;
    case 0x14:
        INC<REGISTER, u8>(this, regs.D);
        break;
    case 0x15:
        DEC<REGISTER, u8>(this, regs.D);
        break;
    case 0x16:
        LD<REGISTER, IMMEDIATE, u8>(this, regs.D, read8(regs.PC++));
        break;
    case 0x18:
        JR(this, static_cast<s8>(read8(regs.PC++)));
        break;
    case 0x19:
        ADD_HLrr(this, regs.DE);
        break;
    case 0x1A:
        LD<REGISTER, IMMEDIATE, u8>(this, regs.A, read8(regs.DE));
        break;
    case 0x1B:
        DEC<REGISTER, u16>(this, regs.DE);
        break;
    case 0x1C:
        INC<REGISTER, u8>(this, regs.E);
        break;
    case 0x1D:
        DEC<REGISTER, u8>(this, regs.E);
        break;
    case 0x1E:
        LD<REGISTER, IMMEDIATE, u8>(this, regs.E, read8(regs.PC++));
        break;
    case 0x20:
        instructionCycles += JR_COND<JP_NZ>(this, static_cast<s8>(read8(regs.PC++)));
        break;
    case 0x21:
        LD<REGISTER, IMMEDIATE, u16>(this, regs.HL, read16(regs.PC));
        regs.PC += 2;
        break;
    case 0x22:
        LD<ADDRESS_PTR, REGISTER, u16>(this, regs.HL, regs.A);
        regs.HL++;
        break;
    case 0x23:
        INC<REGISTER, u16>(this, regs.HL);
        break;
    case 0x24:
        INC<REGISTER, u8>(this, regs.H);
        break;
    case 0x25:
        DEC<REGISTER, u8>(this, regs.H);
        break;
    case 0x26:
        LD<REGISTER, IMMEDIATE, u8>(this, regs.H, read8(regs.PC++));
        break;
    case 0x27:
        DAA(this);
        break;
    case 0x28:
        instructionCycles += JR_COND<JP_Z>(this, static_cast<s8>(read8(regs.PC++)));
        break;
    case 0x29:
        ADD_HLrr(this, regs.HL);
        break;
    case 0x2A:
        LD<REGISTER, IMMEDIATE, u8>(this, regs.A, read8(regs.HL++));
        break;
    case 0x2B:
        DEC<REGISTER, u16>(this, regs.HL);
        break;
    case 0x2C:
        INC<REGISTER, u8>(this, regs.L);
        break;
    case 0x2D:
        DEC<REGISTER, u8>(this, regs.L);
        break;
    case 0x2E:
        LD<REGISTER, IMMEDIATE, u8>(this, regs.L, read8(regs.PC++));
        break;
    case 0x2F:
        CPL(this);
        break;
    case 0x30:
        instructionCycles += JR_COND<JP_NC>(this, static_cast<s8>(read8(regs.PC++)));
        break;
    case 0x31:
        LD<REGISTER, IMMEDIATE, u16>(this, regs.HL, read16(regs.PC));
        regs.PC += 2;
        break;
    case 0x32:
        LD<ADDRESS_PTR, REGISTER, u16>(this, regs.HL, regs.A);
        regs.HL--;
        break;
    case 0x33:
        INC<REGISTER, u16>(this, regs.SP);
        break;
    case 0x34:
        INC<ADDRESS_PTR, u16>(this, regs.HL);
        break;
    case 0x35:
        DEC<ADDRESS_PTR, u16>(this, regs.HL);
        break;
    case 0x36:
        LD<ADDRESS_PTR, IMMEDIATE, u16>(this, regs.HL, read8(regs.PC++));
        break;
    case 0x37:
        SCF(this);
        break;
    case 0x38:
        instructionCycles += JR_COND<JP_C>(this, static_cast<s8>(read8(regs.PC++)));
        break;
    case 0x39:
        ADD_HLrr(this, regs.SP);
        break;
    case 0x3A:
        LD<REGISTER, IMMEDIATE, u8>(this, regs.A, read8(regs.BC--));
        break;
    case 0x3B:
        DEC<REGISTER, u16>(this, regs.SP);
        break;
    case 0x3C:
        INC<REGISTER, u8>(this, regs.A);
        break;
    case 0x3D:
        DEC<REGISTER, u8>(this, regs.A);
        break;
    case 0x3E:
        LD<REGISTER, IMMEDIATE, u8>(this, regs.A, read8(regs.PC++));
        break;
    case 0x3F:
        CCF(this);
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
        LD<REGISTER, IMMEDIATE, u8>(this, regs.L, read8(regs.HL));
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
        LD<ADDRESS_PTR, REGISTER, u16>(this, regs.HL, regs.A);
        break;
    case 0x78:
        LD<REGISTER, REGISTER, u8>(this, regs.A, regs.B);
        break;
    case 0x79:
        LD<REGISTER, REGISTER, u8>(this, regs.A, regs.C);
        break;
    case 0x7A:
        LD<REGISTER, REGISTER, u8>(this, regs.A, regs.D);
        break;
    case 0x7B:
        LD<REGISTER, REGISTER, u8>(this, regs.A, regs.E);
        break;
    case 0x7C:
        LD<REGISTER, REGISTER, u8>(this, regs.A, regs.H);
        break;
    case 0x7D:
        LD<REGISTER, REGISTER, u8>(this, regs.A, regs.L);
        break;
    case 0x7E:
        LD<REGISTER, IMMEDIATE, u8>(this, regs.A, read8(regs.HL));
        break;
    case 0x7F:
        LD<REGISTER, REGISTER, u8>(this, regs.A, regs.A);
        break;
    case 0x80:
        ADDC<REGISTER, u8>(this, regs.B, false);
        break;
    case 0x81:
        ADDC<REGISTER, u8>(this, regs.C, false);
        break;
    case 0x82:
        ADDC<REGISTER, u8>(this, regs.D, false);
        break;
    case 0x83:
        ADDC<REGISTER, u8>(this, regs.E, false);
        break;
    case 0x84:
        ADDC<REGISTER, u8>(this, regs.H, false);
        break;
    case 0x85:
        ADDC<REGISTER, u8>(this, regs.L, false);
        break;
    case 0x86:
        ADDC<IMMEDIATE, u8>(this, read8(regs.HL), false);
        break;
    case 0x87:
        ADDC<REGISTER, u8>(this, regs.A, false);
        break;
    case 0x88:
        ADDC<REGISTER, u8>(this, regs.B, true);
        break;
    case 0x89:
        ADDC<REGISTER, u8>(this, regs.C, true);
        break;
    case 0x8A:
        ADDC<REGISTER, u8>(this, regs.D, true);
        break;
    case 0x8B:
        ADDC<REGISTER, u8>(this, regs.E, true);
        break;
    case 0x8C:
        ADDC<REGISTER, u8>(this, regs.H, true);
        break;
    case 0x8D:
        ADDC<REGISTER, u8>(this, regs.L, true);
        break;
    case 0x8E:
        ADDC<IMMEDIATE, u8>(this, read8(regs.HL), true);
        break;
    case 0x8F:
        ADDC<REGISTER, u8>(this, regs.A, true);
        break;
    case 0x90:
        SUBC<REGISTER, u8>(this, regs.B, false);
        break;
    case 0x91:
        SUBC<REGISTER, u8>(this, regs.C, false);
        break;
    case 0x92:
        SUBC<REGISTER, u8>(this, regs.D, false);
        break;
    case 0x93:
        SUBC<REGISTER, u8>(this, regs.E, false);
        break;
    case 0x94:
        SUBC<REGISTER, u8>(this, regs.H, false);
        break;
    case 0x95:
        SUBC<REGISTER, u8>(this, regs.L, false);
        break;
    case 0x96:
        SUBC<IMMEDIATE, u8>(this, read8(regs.HL), false);
        break;
    case 0x97:
        SUBC<REGISTER, u8>(this, regs.A, false);
        break;
    case 0x98:
        SUBC<REGISTER, u8>(this, regs.B, true);
        break;
    case 0x99:
        SUBC<REGISTER, u8>(this, regs.C, true);
        break;
    case 0x9A:
        SUBC<REGISTER, u8>(this, regs.D, true);
        break;
    case 0x9B:
        SUBC<REGISTER, u8>(this, regs.E, true);
        break;
    case 0x9C:
        SUBC<REGISTER, u8>(this, regs.H, true);
        break;
    case 0x9D:
        SUBC<REGISTER, u8>(this, regs.L, true);
        break;
    case 0x9E:
        SUBC<IMMEDIATE, u8>(this, read8(regs.HL), true);
        break;
    case 0x9F:
        SUBC<REGISTER, u8>(this, regs.A, true);
        break;
    case 0xA0:
        BITWISE_OP<AND, REGISTER, u8>(this, regs.B);
        break;
    case 0xA1:
        BITWISE_OP<AND, REGISTER, u8>(this, regs.C);
        break;
    case 0xA2:
        BITWISE_OP<AND, REGISTER, u8>(this, regs.D);
        break;
    case 0xA3:
        BITWISE_OP<AND, REGISTER, u8>(this, regs.E);
        break;
    case 0xA4:
        BITWISE_OP<AND, REGISTER, u8>(this, regs.H);
        break;
    case 0xA5:
        BITWISE_OP<AND, REGISTER, u8>(this, regs.L);
        break;
    case 0xA6:
        BITWISE_OP<AND, ADDRESS_PTR, u16>(this, regs.HL);
        break;
    case 0xA7:
        BITWISE_OP<AND, REGISTER, u8>(this, regs.A);
        break;
    case 0xA8:
        BITWISE_OP<XOR, REGISTER, u8>(this, regs.B);
        break;
    case 0xA9:
        BITWISE_OP<XOR, REGISTER, u8>(this, regs.C);
        break;
    case 0xAA:
        BITWISE_OP<XOR, REGISTER, u8>(this, regs.D);
        break;
    case 0xAB:
        BITWISE_OP<XOR, REGISTER, u8>(this, regs.E);
        break;
    case 0xAC:
        BITWISE_OP<XOR, REGISTER, u8>(this, regs.H);
        break;
    case 0xAD:
        BITWISE_OP<XOR, REGISTER, u8>(this, regs.L);
        break;
    case 0xAE:
        BITWISE_OP<XOR, IMMEDIATE, u16>(this, regs.HL);
        break;
    case 0xAF:
        BITWISE_OP<XOR, REGISTER, u8>(this, regs.A);
        break;
    case 0xB0:
        BITWISE_OP<OR, REGISTER, u8>(this, regs.B);
        break;
    case 0xB1:
        BITWISE_OP<OR, REGISTER, u8>(this, regs.C);
        break;
    case 0xB2:
        BITWISE_OP<OR, REGISTER, u8>(this, regs.D);
        break;
    case 0xB3:
        BITWISE_OP<OR, REGISTER, u8>(this, regs.E);
        break;
    case 0xB4:
        BITWISE_OP<OR, REGISTER, u8>(this, regs.H);
        break;
    case 0xB5:
        BITWISE_OP<OR, REGISTER, u8>(this, regs.L);
        break;
    case 0xB6:
        BITWISE_OP<OR, IMMEDIATE, u16>(this, regs.HL);
        break;
    case 0xB7:
        BITWISE_OP<OR, REGISTER, u8>(this, regs.A);
        break;
    case 0xB8:
        CP<REGISTER, u8>(this, regs.B);
        break;
    case 0xB9:
        CP<REGISTER, u8>(this, regs.C);
        break;
    case 0xBA:
        CP<REGISTER, u8>(this, regs.D);
        break;
    case 0xBB:
        CP<REGISTER, u8>(this, regs.E);
        break;
    case 0xBC:
        CP<REGISTER, u8>(this, regs.H);
        break;
    case 0xBD:
        CP<REGISTER, u8>(this, regs.L);
        break;
    case 0xBE:
        CP<ADDRESS_PTR, u16>(this, regs.HL);
        break;
    case 0xBF:
        CP<REGISTER, u8>(this, regs.A);
        break;
    case 0xC1:
        POP(this, regs.BC);
        break;
    case 0xC2:
        instructionCycles += JP_COND<JP_NZ>(this, read16(regs.PC));
        regs.PC += 2;
        break;
    case 0xC3:
        JP(this, read16(regs.PC));
        regs.PC += 2;
        break;
    case 0xC5:
        PUSH(this, regs.BC);
        break;
    case 0xC6:
        ADDC<IMMEDIATE, u8>(this, read8(regs.PC++), false);
        break;
    case 0xCA:
        instructionCycles += JP_COND<JP_Z>(this, read16(regs.PC));
        regs.PC += 2;
        break;
    case 0xCB:
        //u8 cbOpcode = read8(regs.PC++);
        decodeAndExecuteCBInstruction(read8(regs.PC++));
        break;
    case 0xCE:
        ADDC<IMMEDIATE, u8>(this, read8(regs.PC++), true);
        break;
    case 0xD1:
        POP(this, regs.DE);
        break;
    case 0xD2:
        instructionCycles += JP_COND<JP_NC>(this, read16(regs.PC));
        regs.PC += 2;
        break;
    case 0xD5:
        PUSH(this, regs.DE);
        break;
    case 0xD6:
        SUBC<IMMEDIATE, u8>(this, read8(regs.PC++), false);
        break;
    case 0xDA:
        instructionCycles += JP_COND<JP_C>(this, read16(regs.PC));
        regs.PC += 2;
        break;
    case 0xDE:
        SUBC<IMMEDIATE, u8>(this, read8(regs.PC++), true);
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
    case 0xE6:
        BITWISE_OP<AND, IMMEDIATE, u8>(this, read8(regs.PC++));
        break;
    case 0xE8:
        ADD_SPi8(this, static_cast<s8>(read8(regs.PC++)));
        break;
    case 0xE9:
        JP(this, regs.HL);
        break;
    case 0xEA:
        {
            u16 address = read16(regs.PC);
            LD<ADDRESS_PTR, REGISTER, u16>(this, address, regs.A);
            regs.PC += 2;
        }
        break;
    case 0xEE:
        BITWISE_OP<XOR, IMMEDIATE, u8>(this, read8(regs.PC++));
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
    case 0xF3:
        DI(system);
        break;
    case 0xF5:
        PUSH(this, regs.AF);
        break;
    case 0xF6:
        BITWISE_OP<OR, IMMEDIATE, u8>(this, read8(regs.PC++));
        break;
    case 0xF8:
        LD_HLSPi8(this, static_cast<s8>(read8(regs.PC++)));
        break;
    case 0xF9:
        LD<REGISTER, REGISTER, u16>(this, regs.SP, regs.HL);
        break;
    case 0xFA:
        LD<REGISTER, IMMEDIATE, u8>(this, regs.A, read8(regs.PC));
        regs.PC += 2;
        break;
    case 0xFB:
        EI(system);
        break;
    case 0xFE:
        CP<IMMEDIATE, u8>(this, read8(regs.PC++));
        break;
    default:
        printf("\nOpcode %02X not implemented.\n", opcode);
        break;
    }
}

auto gb::SM83CPU::decodeAndExecuteCBInstruction(u8 cbOpcode) -> void
{
}
