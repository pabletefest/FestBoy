#include "cpuSM83.h"

#include <cassert>

enum OperandsType
{
    REGISTER,
    ADDRESS_PTR,
    IMMEDIATE
};

template<OperandsType DST_TYPE, OperandsType SRC_TYPE, typename Operand>
constexpr auto LD(gb::SM83CPU* cpu, Operand& dst, const Operand& src) -> void
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

auto PUSH(gb::SM83CPU* cpu, const u16& src) -> void
{
    // cpu->SP -= 2;
    // cpu->write16(cpu->SP, src);

    cpu->write8(--cpu->regs.SP, (src >> 8) & 0x00FF);
    cpu->write8(--cpu->regs.SP, src & 0x00FF);
}

auto POP(gb::SM83CPU* cpu, u16& dst) -> void
{
    // cpu->read16(cpu->SP, dst);
    // cpu->SP += 2;

    dst = cpu->read8(cpu->regs.SP++) & 0x00FF;
    dst |= (static_cast<u16>(cpu->read8(cpu->regs.SP++)) << 8) & 0xFF00;
}

template<OperandsType SRC_TYPE, typename Operand>
auto ADDC(gb::SM83CPU* cpu, const Operand& src, bool addCarryBit = false) -> void
{
    u16 result = 0;

    if constexpr ((SRC_TYPE == REGISTER && std::is_unsigned_v<Operand> && not std::is_rvalue_reference_v<Operand>) || SRC_TYPE == IMMEDIATE)
        result = (std::is_same_v<Operand, u16>) ? src : static_cast<u8>(src) & 0x00FF;
    else if constexpr (SRC_TYPE == ADDRESS_PTR)
        result = (std::is_same_v<Operand, u16>) ? cpu->read16(src) : cpu->read8(src) & 0x00FF;
    else
        assert(false && "Error in ADD/ADC opcode: Possible errors are wrong OperantType, register passed is not unsigned or it's a temporary variable");

    result = cpu->regs.A + ((addCarryBit) ? cpu->regs.flags.C : 0);
    cpu->regs.A = result;

    cpu->setFlag(gb::Z, result == 0);
    cpu->setFlag(gb::N, 0);
    cpu->setFlag(gb::H, result & 0x0001);
    cpu->setFlag(gb::C, result & 0x0080);
}

auto ADD_SPi8(gb::SM83CPU* cpu, const s8& immediate) -> void
{
    cpu->regs.SP += immediate;

    cpu->setFlag(gb::Z, 0);
    cpu->setFlag(gb::N, 0);
    cpu->setFlag(gb::H, cpu->regs.SP & 0x0001);
    cpu->setFlag(gb::C, cpu->regs.SP & 0x0080);
}

auto LD_HLSPi8(gb::SM83CPU* cpu, const s8& immediate) -> void
{
    cpu->regs.HL = cpu->regs.SP + immediate;

    cpu->setFlag(gb::Z, 0);
    cpu->setFlag(gb::N, 0);
    cpu->setFlag(gb::H, cpu->regs.HL & 0x0001);
    cpu->setFlag(gb::C, cpu->regs.HL & 0x0080);
}

auto ADD_HLrr(gb::SM83CPU* cpu, const u16& reg) -> void
{
    cpu->regs.HL += reg;

    //cpu->setFlag(gb::Z, 0);
    cpu->setFlag(gb::N, 0);
    cpu->setFlag(gb::H, cpu->regs.HL & 0x0001);
    cpu->setFlag(gb::C, cpu->regs.HL & 0x0080);
}