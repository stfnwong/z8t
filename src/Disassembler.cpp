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
std::string dis_instr_to_repr(const std::vector<uint8_t>& code_buffer)
{
    std::ostringstream oss;
    std::string instr_repr;
    uint8_t instr_size;
    uint8_t code;
    unsigned int pc = 0;

    while(pc < code_buffer.size())
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
            pc += instr_size;
        }
        else
        {
            std::cerr << "[" << __func__ << "] no instruction starting with byte " 
                << std::hex << unsigned(code) << std::endl;
            break;
        }
    }

    return oss.str();
}


/*
 * dis_instr_to_program()
 */
Program dis_instr_to_program(const std::vector<uint8_t>& code_buffer, uint32_t start_addr)
{
    Program prog_out;
    Instr cur_instr;
    uint8_t code;
    unsigned int pc = 0;

    while(pc < code_buffer.size())
    {
        code = code_buffer[pc];
        auto instr = code_to_instr_repr.find(code);
        if(instr != code_to_instr_repr.end())
        {
            cur_instr.init();
            cur_instr.size = instr->second.second;
            cur_instr.ins = code;

            if(cur_instr.size > 1)
                cur_instr.ins = (cur_instr.ins << 8) | code_buffer[pc + 1];
            if(cur_instr.size > 2)
                cur_instr.ins = (cur_instr.ins << 8) | code_buffer[pc + 2];
                
            cur_instr.adr = start_addr + pc;;

            pc += cur_instr.size;
            prog_out.add(cur_instr);
        }
        else
        {
            std::cerr << "[" << __func__ << "] no instruction starting with byte " 
                << std::hex << unsigned(code) << std::endl;
            break;
        }
    }

    return prog_out;
}

/*
 * dis_instr_to_source_info()
 */
SourceInfo dis_instr_to_source_info(const std::vector<uint8_t>& code_buffer, uint32_t start_addr)
{
    SourceInfo info;

    return info;
}
