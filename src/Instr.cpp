/*
 * INSTR
 * Instruction decode 
 *
 */

#include "Instr.hpp"

uint8_t decode_instr_size(uint8_t instr)
{
    auto s = instr_size_map.find(instr);
    if(s != instr_size_map.end())
        return s->second;

    return 0;
}
