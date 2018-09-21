/*
 * ASSEMBLER
 * Z80 assembler 
 *
 * Stefan Wong 2018
 */

#ifndef __ASSEMBLER_HPP
#define __ASSEMBLER_HPP

#include <string>

class Assembler
{
    private:
        bool verbose;

    private:
        // assembly routines

    public:
        Assembler();
        ~Assembler();
        Assembler(const Assembler& that) = delete;


        void assemble(void);

        void setVerbose(const bool v);
        bool getVerbose(void) const;
};

#endif /*__ASSEMBLER_HPP*/
