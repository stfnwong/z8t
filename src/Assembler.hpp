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

    public:
        Assembler();
        ~Assembler();
        Assembler(const Assembler& that) = delete;


        void assemble(void);

        void loadSource(const SourceInfo& s);
        void setVerbose(const bool v);
        bool getVerbose(void) const;

        // getters, setters 
        Program getProgram(void) const;
};

#endif /*__ASSEMBLER_HPP*/
