#include "cpuSM83.h"
#include <assert.h>

enum OperandsType
{
    REGISTER,
    ADDRESS_PTR,
    IMMEDIATE
};

enum BitWidth
{
    x8,
    x16
};

template<OperandsType DST_TYPE, OperandsType SRC_TYPE, BitWidth BITS_WIDTH,
    typename DST, typename SRC>
constexpr auto LD(gb::SM83CPU* cpu, DST& dst, const SRC& src) -> void
{
    u16 value = 0;
    
    if constexpr ((SRC_TYPE == REGISTER && std::is_unsigned<SRC>::value && not std::is_rvalue_reference<DST>::value) || SRC_TYPE == IMMEDIATE)
        value = (BITS_WIDTH == 16) ? src : static_cast<u8>(src) & 0x00FF;
    else if constexpr (SRC_TYPE == ADDRESS_PTR)
        value = (BITS_WIDTH == 16) ? cpu->read16(src) : cpu->read8(src) & 0x00FF;
    else
        assert(false && "Error in LD opcode: Possible errors are wrong OperantType, register passed is not unsigned or it's a temporary variable");

    if constexpr (DST_TYPE == REGISTER && std::is_unsigned<DST>::value && not std::is_rvalue_reference<DST>::value)
        dst = (BITS_WIDTH == 16) ? value : static_cast<u8>(value) & 0x00FF;
    else if constexpr (DST_TYPE == ADDRESS_PTR || DST_TYPE == IMMEDIATE)
        (BITS_WIDTH == 16) ? cpu->write16(dst, value) : cpu->write8(dst, static_cast<u8>(value));
    else
        assert(false && "Error in LD opcode: Possible errors are wrong OperantType, register passed is not unsigned or it's a temporary variable");
}