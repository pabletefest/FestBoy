#include "cpuSM83.h"
#include <assert.h>

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