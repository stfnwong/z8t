/* OPCODE 
 * Objects for dealing with opcodes 
 *
 * Stefan Wong 2018
 */

#ifndef __OPCODE_HPP
#define __OPCODE_HPP

#include <cstdint.h>
#include <string>

typedef struct 
{
    public:
        std::string mnemonic;
        uint16_t    code;
} Opcode;


/*
 * Instruction table 
 */
class InstrTable
{

};

#endif /*__OPCODE_HPP*/
