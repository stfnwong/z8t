/*
 * DISASSEMBLER
 *
 */

#include <iostream>
#include <iomanip>
#include <sstream>
#include "Disassembler.hpp"
#include "Instruction.hpp"

// TODO: possibly write a version that accepts a vector of
// uint8_t (may need a conversion step)

/*
 * dis_program()
 */
//TextLine dis_program(const uint8_t *code_buffer, int pc)
//{
//}

/*
 * dis_instr_to_repr()
 */
std::string dis_instr_to_repr(uint8_t *code_buffer, int buf_size)
{
    std::ostringstream oss;
    std::string instr_repr;
    uint8_t instr_size;
    uint8_t code;
    int pc = 0;

    while(pc < buf_size)
    {
        code = code_buffer[pc];
        auto instr = code_to_instr_repr.find(code);
        if(instr != code_to_instr_repr.end())
        {
            instr_repr = instr->second.first;
            instr_size = instr->second.second;

            // TODO : for [ld (**) hl] there will need to be a special case here since
            // the literal arg comes first
            oss << instr_repr;

            // consume one more byte from the stream
            if(instr_size > 1)
                oss << "$" << std::hex << unsigned(code_buffer[pc+1]);
            if(instr_size > 2)
                oss << std::hex << unsigned(code_buffer[pc+2]);

            oss << std::endl;
        }
        else
        {
            std::cerr << "[" << __func__ << "] no instruction startig with byte " 
                << std::hex << unsigned(code) << std::endl;
            break;
        }
        pc += instr_size;
    }

    return oss.str();
}
