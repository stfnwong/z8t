/*
 * DISASSEMBLER
 *
 */

#ifndef __DISASSEMBLER_HPP
#define __DISASSEMBLER_HPP

#include <cstdint>
#include <string>
#include <vector>

#include "Program.hpp"
#include "Source.hpp"


/*
 * dis_program()
 * Given a codebuffer stream, consume bytes and emit a TextLine 
 * that represents the given instruction
 */
//TextLine dis_program(const uint8_t *code_buffer, int pc);

/*
 * dis_instr_to_repr()
 * Given a codebuffer stream, consume bytes and emit string representatons 
 * for each instruction.
 */
// TODO: may have to make this a std::vector<uint8_t> instead...
std::string dis_instr_to_repr(const std::vector<uint8_t>& code_buffer);

/*
 * dis_instr_to_program()
 * Accepts a vector of program bytes and returns a program object.
 */
Program dis_instr_to_program(const std::vector<uint8_t>& code_buffer, uint32_t start_addr);

/*
 * dis_instr_to_source_info()
 * Accepts a vector of program bytes and returns a SourceInfo object
 */
SourceInfo dis_instr_to_source_info(const std::vector<uint8_t>& code_buffer, uint32_t start_addr);


#endif /*__DISASSEMBLER_HPP*/
