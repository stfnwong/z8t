/*
 * DISASSEMBLER
 *
 */

#include <iostream>
#include <iomanip>
#include "Disassembler.hpp"

// TODO: possibly write a version that accepts a vector of
// uint8_t (may need a conversion step)

// TODO : this is instr to repr, not instr to TextLine
/*
 * dis_program()
 */
TextLine dis_program(const uint8_t *code_buffer, int buf_size)
{
    TextLine line;
    uint8_t code;
    int op_size = 1;
    int pc = 0;

    while(pc < buf_size)
    {
        code = code_buffer[pc];

        auto instr_lookup = dis_instr_lookup.find(code);
        if(instr_lookup != dis_instr_lookup.end())
        {
            uint8_t instr_size     = instr_lookup->second.second;
            std::string instr_repr = instr_lookup->second.first;

            // format the output line
            line.init();
        }
        else
        {
            std::cerr << "[" << __func__ << "] no instruction starting with byte " 
                << std::hex << unsigned(code) << std::endl;
            break;
        }

        pc += instr_lookup->second.second;
    }


    return line;
}
