/*
 * DISASSEMBLER
 *
 */

#ifndef __DISASSEMBLER_HPP
#define __DISASSEMBLER_HPP

#include <cstdint>
#include <string>
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
std::string dis_instr_to_repr(uint8_t *code_buffer, int buf_size);


#endif /*__DISASSEMBLER_HPP*/
