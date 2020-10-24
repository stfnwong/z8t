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
 * asm_add()
 */
Instr Assembler::asm_add(const TextLine& line) const
{
    Instr instr;

    instr.adr = line.addr;
    if(line.args[0].type == SYM_REG && line.args[1].type == SYM_REG)
    {
        if(line.args[0].val == REG_A)
        {
            switch(line.args[1].val)
            {
                case REG_A:
                    instr.ins = 0x87;
                    break;
                case REG_B:
                    instr.ins = 0x80;
                    break;
                case REG_C:
                    instr.ins = 0x81;
                    break;
                case REG_D:
                    instr.ins = 0x82;
                    break;
                case REG_E:
                    instr.ins = 0x83;
                    break;
                case REG_H:
                    instr.ins = 0x84;
                    break;
                case REG_L:
                    instr.ins = 0x85;
                    break;
                case REG_HL_IND:
                    instr.ins = 0x86;
                    break;
            }
        }
        else if(line.args[0].val == REG_HL)
        {
            switch(line.args[1].val)
            {
                case REG_BC:
                    instr.ins = 0x09;
                    break;
                case REG_DE:
                    instr.ins = 0x19;
                    break;
                case REG_HL:
                    instr.ins = 0x29;
                    break;
                case REG_SP:
                    instr.ins = 0x39;
                    break;
            }
        }
    }
    else if(line.args[0].type == SYM_REG && line.args[0].type == SYM_LITERAL)
    {
        instr.ins = (0xC6 << 2) | (line.args[1].val & 0xFF);
    }

    if(instr.ins == 0)
        std::cerr << "[" << __func__ << "] invalid operands for instruction " << line.toInstrString() << std::endl;

    return instr;
}

/*
 * asm_and()
 */
Instr Assembler::asm_and(const TextLine& line) const
{
    Instr instr;

    instr.adr = line.addr;
    if(line.args[0].type == SYM_REG)
    {
        switch(line.args[0].val)
        {
            case REG_A:
                instr.ins = 0xA7;
                break;
            case REG_B:
                instr.ins = 0xA0;
                break;
            case REG_C:
                instr.ins = 0xA1;
                break;
            case REG_D:
                instr.ins = 0xA2;
                break;
            case REG_E:
                instr.ins = 0xA3;
                break;
            case REG_H:
                instr.ins = 0xA4;
                break;
            case REG_L:
                instr.ins = 0xA5;
                break;
        }
    }
    else if(line.args[0].type == SYM_LITERAL)
    {
        instr.ins = 0xE6;
    }

    if(instr.ins == 0)
        std::cerr << "[" << __func__ << "] invalid operands for instruction " << line.toInstrString() << std::endl;

    return instr;
}

/*
 * asm_dec()
 */
Instr Assembler::asm_dec(const TextLine& line) const
{
    Instr instr;

    instr.adr = line.addr;
    switch(line.args[0].val)
    {
        case REG_A:
            instr.ins = 0x3D;
            break;
        case REG_B:
            instr.ins = 0x05;
            break;
        case REG_C:
            instr.ins = 0x0D;
            break;
        case REG_D:
            instr.ins = 0x15;
            break;
        case REG_E:
            instr.ins = 0x1D;
            break;
        case REG_H:
            instr.ins = 0x25;
            break;
        case REG_L:
            instr.ins = 0x2D;
            break;
        case REG_BC:
            instr.ins = 0x0B;
            break;
        case REG_DE:
            instr.ins = 0x1B;
            break;
        case REG_HL:
            instr.ins = 0x2B;
            break;
        case REG_SP:
            instr.ins = 0x3B;
            break;
        case REG_HL_IND:
            instr.ins = 0x35;
            break;
    }

    if(instr.ins == 0)
        std::cerr << "[" << __func__ << "] invalid operands for instruction " << line.toInstrString() << std::endl;

    return instr;
}

/*
 * asm_ld()
 */
// TODO: this is actually quite ugly...
Instr Assembler::asm_ld(const TextLine& line) const
{
    Instr instr;

    instr.adr = line.addr;
    switch(line.args[0].val)
    {
        case REG_A:
        {
            switch(line.args[1].val)
            {
                case REG_A:
                    instr.ins = 0x7F;
                    break;
                case REG_B:
                    instr.ins = 0x79;
                    break;
                case REG_C:
                    instr.ins = 0x79;
                    break;
                case REG_D:
                    instr.ins = 0x7A;
                    break;
                case REG_E:
                    instr.ins = 0x7B;
                    break;
                case REG_H:
                    instr.ins = 0x7C;
                    break;
                case REG_L:
                    instr.ins = 0x7D;
                    break;
                case REG_HL_IND:
                    instr.ins = 0x7E;
                    break;
                case REG_BC_IND:
                    instr.ins = 0x0A;
                    break;
                case REG_DE_IND:
                    instr.ins = 0x1A;
                    break;
            }
            break;
        }
        case REG_B:
            break;

        if(line.args[0].type == SYM_LITERAL)
        {

        }
    }


    return instr;
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
            cur_instr.ins = lookup_val->second;
        else
        {
            std::cerr << "[" << __func__ << "] skipping instruction " << cur_line.toInstrString() 
                << " with hash " << std::hex << line_hash << std::endl;
            continue;
        }
        cur_instr.adr = cur_line.addr;

        std::cout << "[" << __func__ << "] assembled instruction " << cur_line.toInstrString() << " with hash 0x" << std::hex << line_hash 
            << " cur_instr : " << cur_instr.toString() << std::endl;
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

