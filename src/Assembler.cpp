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
    this->init();
}

void Assembler::init(void)
{
    this->source.init();
    this->program.init();
}


/*
 * assemble
 */
void Assembler::assemble(void)
{
    TextLine cur_line;

    if(this->source.getNumLines() == 0)
    {
        std::cout << "[" << __func__ << "] error - no lines in source" << std::endl;
        return;
    }
    this->program.init();

    // TODO ; just worry about stuff that goes in the text section, not sure what the 
    // data section layout actually is yet..
    for(unsigned int i = 0; i < this->source.getNumLines(); ++i)
    {
        Instr cur_instr;
        uint32_t line_hash;

        cur_line  = this->source.get(i);
        line_hash = cur_line.argHash();

        auto lookup_val = instr_lookup.find(line_hash);
        if(lookup_val != instr_lookup.end())
        {
            auto instr_size = lookup_val->second;
            cur_instr.size = instr_size.second;
            if(cur_instr.size == 1)
                cur_instr.ins  = instr_size.first;
            else if(cur_instr.size == 2 && cur_line.args[0].type == SYM_LITERAL)
                cur_instr.ins = (instr_size.first << 8) | (cur_line.args[0].val & 0xFF);
            else if(cur_instr.size == 2 && cur_line.args[1].type == SYM_LITERAL)
                cur_instr.ins = (instr_size.first << 8) | (cur_line.args[1].val & 0xFF);
            // ld bc, ** | ld de, ** | ld hl, ** | ld sp, **
            else if(cur_instr.size == 3 && cur_line.args[1].type == SYM_LITERAL)
                cur_instr.ins = (instr_size.first << 16) | (cur_line.args[1].val & 0xFFFF);
            else if(cur_instr.size == 3 && cur_line.args[0].type == SYM_LITERAL_IND)
                cur_instr.ins = (instr_size.first << 16) | (cur_line.args[0].val & 0xFFFF);
        }
        else
        {
            std::cerr << "[" << __func__ << "] skipping instruction " << cur_line.toInstrString() 
                << " with hash " << std::hex << line_hash << std::endl;
            continue;
        }
        cur_instr.adr = cur_line.addr;
        this->program.add(cur_instr);
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
