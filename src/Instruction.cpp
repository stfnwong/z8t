/*
 * INSTRUCTION
 * Lookups for instructions
 *
 */

#include "Instruction.hpp"

/*
 * Find the size of an instruction in bytes given its hash
 */
uint8_t instr_get_size(uint32_t arg_hash)
{
    auto lookup_val = instr_hash_to_code.find(arg_hash);
    if(lookup_val != instr_hash_to_code.end())
        return lookup_val->second.second;

    return 0;
}
