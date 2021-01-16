/*
 * PROGRAM
 * Binary utils for program
 */


#ifndef __PROGRAM_HPP
#define __PROGRAM_HPP

#include <string>
#include <vector>
#include "Source.hpp"


/* 
 * Instr
 * Instruction object
 */
struct Instr
{
    uint16_t adr;
    uint32_t ins;
    uint8_t  size;

    public:
        Instr();
        Instr(uint16_t adr, uint32_t ins);
        Instr(uint16_t adr, uint32_t ins, uint8_t size);

        bool operator==(const Instr& that) const;
        bool operator!=(const Instr& that) const;

        /*
         * init()
         * Reset instruction
         */
        void init(void);
        /*
         * getInstr()
         * Get the instruction without checking its size.
         */
        uint32_t getInstr(void) const;  
        /*
         * toString()
         * Return a string representation of the instruction.
         */
        std::string toString(void) const;
};


/* 
 * Program
 * Object holding output of assembler.
 */
class Program
{
    private:
        bool verbose;
        std::vector<Instr> instructions;

    private:
        // string conversion
        std::string instr_to_string(const Instr& i);

    public:
        Program();
        ~Program();
        Program(const Program& that);

        bool operator==(const Program& that) const;
        bool operator!=(const Program& that) const;

        /*
         * init()
         * Reset the program object
         */
        void init(void);
        /*
         * get()
         * Return the instruction at position idx of the 
         * instruction vector. If no such vector exists, 
         * an empty instruction is returned.
         */
        Instr get(const unsigned int idx) const;
        /*
         * getStr()
         * Return the string representation of the instruction
         * at position idx of the instruction vector. 
         */
        std::string getStr(const unsigned idx);
        /*
         * add()
         * Add an instruction to the end of the program.
         */
        void add(const Instr& i);

        /*
         * writeMem()
         * Add the value in val to the address adr in the program.
         * This method creates a new Instr structure containing the
         * value and address and appends it to the end of the 
         * instruction vector.
         */
        void writeMem(const uint16_t addr, const uint16_t val);

        /*
         * length()
         * Returns the number of instructions in the program.
         */
        unsigned int length(void) const;

        /*
         * numBytes()
         */
        unsigned int numBytes(void) const;
        /*
         * startAddr()
         * Returns first address in the program
         */
         uint16_t startAddr(void) const;
        /*
         * save()
         * 
         * Inputs: std::string filename
         * Write the program binary to the file given by filename
         */
        int save(const std::string& filename);
        /* 
         * load()
         * Load the object file with name filename into memory as 
         * a Program object.
         */
        int load(const std::string& filename);

        /*
         * writeObj()
         */
        int writeObj(const std::string& filename);
        /*
         * readObj()
         */
        int readObj(const std::string& filename);
        std::vector<uint8_t> toArray(void) const;
        /*
         * toString()
         * Render the program object as a string
         */
        std::string toString(void) const;

        void setVerbose(const bool v);
        bool getVerbose(void) const;

};

#endif /*__PROGRAM_HPP*/
