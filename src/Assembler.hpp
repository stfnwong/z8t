/*
 * ASSEMBLER
 * Z80 assembler 
 *
 * Stefan Wong 2018
 */

#ifndef __ASSEMBLER_HPP
#define __ASSEMBLER_HPP

#include <string>
#include "Source.hpp"
#include "Program.hpp"

class Assembler
{
    private:
        bool verbose;
        SourceInfo source;
        Program program;

    private:
        // assembly routines
        Instr asm_add(const TextLine& line) const;
        Instr asm_and(const TextLine& line) const;
        Instr asm_dec(const TextLine& line) const;
        Instr asm_ld(const TextLine& line) const;

    public:
        Assembler();

        void init(void);
        void assemble(void);

        void loadSource(const SourceInfo& s);
        void setVerbose(const bool v);
        bool getVerbose(void) const;

        // getters, setters 
        Program getProgram(void) const;
};

#endif /*__ASSEMBLER_HPP*/
