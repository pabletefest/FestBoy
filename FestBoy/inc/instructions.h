#pragma once
#include "cpuSM83.h"
#include "gb.h"

#include <cassert>

namespace gb
{
    enum OperandsType
    {
        REGISTER,
        ADDRESS_PTR,
        IMMEDIATE
    };

    enum BitwiseOperation
    {
        AND,
        XOR,
        OR
    };

    enum JumpConditionFlags
    {
        JP_NZ,
        JP_Z,
        JP_NC,
        JP_C
    };

    template<OperandsType DST_TYPE, OperandsType SRC_TYPE, typename Operand>
    static constexpr auto LD(gb::SM83CPU* cpu, Operand& dst, const Operand& src) -> void
    {
        u16 value = 0;

        if constexpr ((SRC_TYPE == REGISTER && std::is_unsigned_v<Operand> && not std::is_rvalue_reference_v<Operand>) || SRC_TYPE == IMMEDIATE)
            value = (std::is_same_v<Operand, u16>) ? src : static_cast<u8>(src) & 0x00FF;
        else if constexpr (SRC_TYPE == ADDRESS_PTR)
            value = (std::is_same_v<Operand, u16>) ? cpu->read16(src) : cpu->read8(src) & 0x00FF;
        else
            assert(false && "Error in LD opcode: Possible errors are wrong OperantType, register passed is not unsigned or it's a temporary variable");

        if constexpr (DST_TYPE == REGISTER && std::is_unsigned_v<Operand> && not std::is_rvalue_reference_v<Operand>)
            dst = (std::is_same_v<Operand, u16>) ? value : static_cast<u8>(value) & 0x00FF;
        else if constexpr (DST_TYPE == ADDRESS_PTR || DST_TYPE == IMMEDIATE)
            (std::is_same_v<Operand, u16>) ? cpu->write16(dst, value) : cpu->write8(dst, static_cast<u8>(value));
        else
            assert(false && "Error in LD opcode: Possible errors are wrong OperantType, register passed is not unsigned or it's a temporary variable");
    }

    static auto PUSH(gb::SM83CPU* cpu, const u16& src) -> void
    {
        // cpu->SP -= 2;
        // cpu->write16(cpu->SP, src);

        cpu->write8(--cpu->regs.SP, (src >> 8) & 0x00FF);
        cpu->write8(--cpu->regs.SP, src & 0x00FF);
    }

    static auto POP(gb::SM83CPU* cpu, u16& dst) -> void
    {
        // cpu->read16(cpu->SP, dst);
        // cpu->SP += 2;

        dst = cpu->read8(cpu->regs.SP++) & 0x00FF;
        dst |= (static_cast<u16>(cpu->read8(cpu->regs.SP++)) << 8) & 0xFF00;
    }

    template<OperandsType SRC_TYPE, typename Operand>
    static constexpr auto ADDC(gb::SM83CPU* cpu, const Operand& src, bool addCarryBit = false) -> void
    {
        u16 result = 0;

        if constexpr ((SRC_TYPE == REGISTER && std::is_unsigned_v<Operand> && not std::is_rvalue_reference_v<Operand>) || SRC_TYPE == IMMEDIATE)
            result = (std::is_same_v<Operand, u16>) ? src : static_cast<u8>(src) & 0x00FF;
        else if constexpr (SRC_TYPE == ADDRESS_PTR)
            result = (std::is_same_v<Operand, u16>) ? cpu->read16(src) : cpu->read8(src) & 0x00FF;
        else
            assert(false && "Error in ADD/ADC opcode: Possible errors are wrong OperantType, register passed is not unsigned or it's a temporary variable");

        result += cpu->regs.A + ((addCarryBit) ? cpu->regs.flags.C : 0);
        cpu->regs.A = result & 0x00FF;

        cpu->setFlag(gb::Z, result == 0);
        cpu->setFlag(gb::N, 0);
        cpu->setFlag(gb::H, result & 0x0001);
        cpu->setFlag(gb::C, result & 0x0080);
    }

    static auto ADD_SPi8(gb::SM83CPU* cpu, const s8& immediate) -> void
    {
        cpu->regs.SP += immediate;

        cpu->setFlag(gb::Z, 0);
        cpu->setFlag(gb::N, 0);
        cpu->setFlag(gb::H, cpu->regs.SP & 0x0001);
        cpu->setFlag(gb::C, cpu->regs.SP & 0x0080);
    }

    static auto LD_HLSPi8(gb::SM83CPU* cpu, const s8& immediate) -> void
    {
        cpu->regs.HL = cpu->regs.SP + immediate;

        cpu->setFlag(gb::Z, 0);
        cpu->setFlag(gb::N, 0);
        cpu->setFlag(gb::H, cpu->regs.HL & 0x0001);
        cpu->setFlag(gb::C, cpu->regs.HL & 0x0080);
    }

    static auto ADD_HLrr(gb::SM83CPU* cpu, const u16& reg) -> void
    {
        cpu->regs.HL += reg;

        //cpu->setFlag(gb::Z, 0);
        cpu->setFlag(gb::N, 0);
        cpu->setFlag(gb::H, cpu->regs.HL & 0x0001);
        cpu->setFlag(gb::C, cpu->regs.HL & 0x0080);
    }

    template<OperandsType SRC_TYPE, typename Operand>
    static constexpr auto SUBC(gb::SM83CPU* cpu, const Operand& src, bool subCarryBit = false) -> void
    {
        u16 operand = 0;
        u16 result = 0;

        if constexpr ((SRC_TYPE == REGISTER && std::is_unsigned_v<Operand> && not std::is_rvalue_reference_v<Operand>) || SRC_TYPE == IMMEDIATE)
            operand = (std::is_same_v<Operand, u16>) ? src : static_cast<u8>(src) & 0x00FF;
        else if constexpr (SRC_TYPE == ADDRESS_PTR)
            operand = (std::is_same_v<Operand, u16>) ? cpu->read16(src) : cpu->read8(src) & 0x00FF;
        else
            assert(false && "Error in SUB/SBD opcode: Possible errors are wrong OperantType, register passed is not unsigned or it's a temporary variable");

        result = cpu->regs.A - operand - ((subCarryBit) ? cpu->regs.flags.C : 0);
        cpu->regs.A = result & 0x00FF;

        cpu->setFlag(gb::Z, result == 0);
        cpu->setFlag(gb::N, 1);
        cpu->setFlag(gb::H, result & 0x0001);
        cpu->setFlag(gb::C, result & 0x0080);
    }

    template<BitwiseOperation OP_TYPE, OperandsType SRC_TYPE, typename Operand>
    static constexpr auto BITWISE_OP(gb::SM83CPU* cpu, const Operand& src) -> void
    {
        u16 operand = 0;
        u16 result = 0;

        if constexpr ((SRC_TYPE == REGISTER && std::is_unsigned_v<Operand> && not std::is_rvalue_reference_v<Operand>) || SRC_TYPE == IMMEDIATE)
            operand = (std::is_same_v<Operand, u16>) ? src : static_cast<u8>(src) & 0x00FF;
        else if constexpr (SRC_TYPE == ADDRESS_PTR)
            operand = (std::is_same_v<Operand, u16>) ? cpu->read16(src) : cpu->read8(src) & 0x00FF;
        else
            assert(false && "Error in AND/XOR/OR opcode: Possible errors are wrong OperantType, register passed is not unsigned or it's a temporary variable");

        if constexpr (OP_TYPE == AND)
            result = cpu->regs.A & operand;
        else if constexpr (OP_TYPE == XOR)
            result = cpu->regs.A ^ operand;
        else if constexpr (OP_TYPE == OR)
            result = cpu->regs.A | operand;
        else
            assert(false && "Error in AND/XOR/OR opcode: specify a valid bitwise operation!");

        cpu->regs.A = static_cast<u8>(result);

        cpu->setFlag(gb::Z, result == 0);
        cpu->setFlag(gb::N, 0);

        if constexpr (OP_TYPE == AND)
            cpu->setFlag(gb::H, 1);
        else
            cpu->setFlag(gb::H, 0);

        cpu->setFlag(gb::C, 0);
    }

    template<OperandsType SRC_TYPE, typename Operand>
    static constexpr auto CP(gb::SM83CPU* cpu, const Operand& src) -> void
    {
        u16 operand = 0;

        if constexpr ((SRC_TYPE == REGISTER && std::is_unsigned_v<Operand> && not std::is_rvalue_reference_v<Operand>) || SRC_TYPE == IMMEDIATE)
            operand = (std::is_same_v<Operand, u16>) ? src : static_cast<u8>(src) & 0x00FF;
        else if constexpr (SRC_TYPE == ADDRESS_PTR)
            operand = (std::is_same_v<Operand, u16>) ? cpu->read16(src) : cpu->read8(src) & 0x00FF;
        else
            assert(false && "Error in CP opcode: Possible errors are wrong OperantType, register passed is not unsigned or it's a temporary variable");

        u16 result = cpu->regs.A - operand;

        cpu->setFlag(gb::Z, result == 0);
        cpu->setFlag(gb::N, 1);
        cpu->setFlag(gb::H, result & 0x0001);
        cpu->setFlag(gb::C, result & 0x0080);
    }

    template<OperandsType SRC_TYPE, typename Operand>
    static constexpr auto INC(gb::SM83CPU* cpu, Operand& src) -> void
    {
        u16 result = 0;

        if constexpr ((SRC_TYPE == REGISTER && std::is_unsigned_v<Operand> && not std::is_rvalue_reference_v<Operand>))
        {
            src += 1;
            result = src;
        }
        else if constexpr (SRC_TYPE == ADDRESS_PTR)
        {
            result = cpu->read8(src) + 1;
            cpu->write8(src, static_cast<u8>(result));
        }
        else
            assert(false && "Error in INC opcode: Possible errors are wrong OperantType, register passed is not unsigned or it's a temporary/immediate variable");

        cpu->setFlag(gb::Z, result == 0);
        cpu->setFlag(gb::N, 0);
        cpu->setFlag(gb::H, result & 0x0001);
    }

    template<OperandsType SRC_TYPE, typename Operand>
    static constexpr auto DEC(gb::SM83CPU* cpu, Operand& src) -> void
    {
        u16 result = 0;

        if constexpr ((SRC_TYPE == REGISTER && std::is_unsigned_v<Operand> && not std::is_rvalue_reference_v<Operand>))
        {
            src -= 1;
            result = src;
        }
        else if constexpr (SRC_TYPE == ADDRESS_PTR)
        {
            result = cpu->read8(src) - 1;
            cpu->write8(src, static_cast<u8>(result));
        }
        else
            assert(false && "Error in DEC opcode: Possible errors are wrong OperantType, register passed is not unsigned or it's a temporary/immediate variable");

        cpu->setFlag(gb::Z, result == 0);
        cpu->setFlag(gb::N, 1);
        cpu->setFlag(gb::H, result & 0x0001);
    }

    static auto DAA(gb::SM83CPU* cpu) -> void
    {
        // We do high nybble first so that adding 0x06 don't modify upper nybble
        if (((cpu->regs.A & 0xF0) > 0x90) || cpu->getFlag(gb::C)) // Also ((cpu->regs.A >> 4) & 0x0F) > 0x09
        {
            cpu->regs.A += (cpu->getFlag(gb::N)) ? -0x60 : 0x60;
        }

        if (((cpu->regs.A & 0x0F) > 0x09) || cpu->getFlag(gb::H))
        {
            cpu->regs.A += (cpu->getFlag(gb::N)) ? -0x06 : 0x06;
        }

        cpu->setFlag(gb::Z, cpu->regs.A == 0);
        cpu->setFlag(gb::H, 0);
        cpu->setFlag(gb::C, cpu->regs.A > 0x99);
    }

    static auto CPL(gb::SM83CPU* cpu) -> void
    {
        cpu->regs.A = ~cpu->regs.A;

        cpu->setFlag(gb::N, 1);
        cpu->setFlag(gb::H, 1);
    }

    static auto CCF(gb::SM83CPU* cpu) -> void
    {
        cpu->setFlag(gb::C, ~cpu->getFlag(gb::C));
        cpu->setFlag(gb::N, 0);
        cpu->setFlag(gb::H, 0);
    }

    static auto SCF(gb::SM83CPU* cpu) -> void
    {
        cpu->setFlag(gb::C, 1);
        cpu->setFlag(gb::N, 0);
        cpu->setFlag(gb::H, 0);
    }

    static auto NOP() -> void
    {
        // Does nothing
    }

    static auto EI(gb::GBConsole* console) -> void
    {
        console->IME = true;
    }

    static auto DI(gb::GBConsole* console) -> void
    {
        console->IME = false;
    }

    static auto JP(gb::SM83CPU* cpu, const u16& address) -> void
    {
        cpu->regs.PC = address;
    }

    template<JumpConditionFlags condition>
    static auto JP_COND(gb::SM83CPU* cpu, const u16& address) -> u8
    {
        u8 extraCycles = 0;
    
        if ((condition == JP_NZ && cpu->getFlag(Z) == 0)
            || (condition == JP_Z && cpu->getFlag(Z) == 1)
            || (condition == JP_NC && cpu->getFlag(C) == 0)
            || (condition == JP_C && cpu->getFlag(C) == 1))
        {
            extraCycles += 4;
            JP(cpu, address);
        }

        return extraCycles;
    }

    static auto JR(gb::SM83CPU* cpu, const s8& relativeAddress) -> void
    {
        cpu->regs.PC += relativeAddress;
    }

    template<JumpConditionFlags condition>
    static auto JR_COND(gb::SM83CPU* cpu, const s8& relativeAddress) -> u8
    {
        u8 extraCycles = 0;

        if ((condition == JP_NZ && cpu->getFlag(Z) == 0)
            || (condition == JP_Z && cpu->getFlag(Z) == 1)
            || (condition == JP_NC && cpu->getFlag(C) == 0)
            || (condition == JP_C && cpu->getFlag(C) == 1))
        {
            extraCycles += 4;
            JR(cpu, relativeAddress);
        }

        return extraCycles;
    }
}