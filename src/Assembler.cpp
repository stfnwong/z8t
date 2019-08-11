/*
 * ASSEMBLER
 * Z80 assembler 
 *
 * Stefan Wong 2018
 */

#include <iostream>
#include "Assembler.hpp"


Assembler::Assembler()
{
    this->verbose = false;
    this->source = SourceInfo();
    this->program = Program();
}


void Assembler::assemble(void)
{
    LineInfo cur_line;

    this->program.initProgram();
    if(this->source.getNumLines() == 0)
    {
        std::cout << "[" << __FUNCTION__ << "] error - no lines in source" << std::endl;
        return;
    }


    for(unsigned int i = 0; i < this->source.getNumLines(); ++i)
    {
        cur_line = this->source.get(i);
        switch(cur_line.opcode.code)
        {

            default:
                if(this->verbose)
                {
                    std::cerr << "[" << __FUNCTION__ << "] (line " << std::dec << cur_line.line_num << 
                        ") unknown lexer code " << std::hex << cur_line.opcode.code << std::endl;

                }
                break;
        }

    }
}


void Assembler::loadSource(const SourceInfo& s)
{
    this->source = s;
}

Program Assembler::getProgram(void) const
{
    return this->program;
}


