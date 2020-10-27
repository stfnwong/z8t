/*
 * PROGRAM
 * Binary utils 
 */

#include <fstream>
#include <sstream>
#include <iomanip>
#include <iostream>
#include "Program.hpp"


// ======== PROGRAM ======== //
Instr::Instr() : adr(0), ins(0), size(0) {} 

Instr::Instr(uint16_t adr, uint32_t ins) : adr(adr), ins(ins), size(1) {} 

Instr::Instr(uint16_t adr, uint32_t ins, uint8_t size) : adr(adr), ins(ins), size(size) {} 

/*
 * ==
 */
bool Instr::operator==(const Instr& that) const
{
    if(this->adr != that.adr)
        return false;
    if(this->ins != that.ins)
        return false;
    if(this->size != that.size)
        return false;

    return true;
}

/*
 * !=
 */
bool Instr::operator!=(const Instr& that) const
{
    return !(*this == that);
}

/*
 * init()
 */
void Instr::init(void)
{
    this->adr = 0;
    this->ins = 0;
    this->size = 0;
}

/*
 * getInstr()
 */
uint32_t Instr::getInstr(void) const
{
    if(this->size == 1)
        return this->ins & 0xFF;
    if(this->size == 2)
        return this->ins & 0xFFFF;

    return this->ins & 0xFFFFFF;
}

/*
 * toString()
 */
std::string Instr::toString(void) const
{
    std::ostringstream oss;

    oss << "[" << std::hex << std::setw(4) << std::setfill('0') 
        << this->adr << "] 0x" << this->ins << " (" 
        << unsigned(this->size) << " bytes)";

    return oss.str();
}


// ======== PROGRAM ======== //
Program::Program() {} 

Program::~Program() {} 

Program::Program(const Program& that)
{
    this->verbose = that.verbose;
    for(unsigned int i = 0; i < that.instructions.size(); ++i)
        this->instructions.push_back(that.instructions[i]);
}


/*
 * instr_to_string()
 */
std::string Program::instr_to_string(const Instr& i)
{
    std::ostringstream oss;

    oss << "[" << std::hex << std::setw(4) << std::setfill('0') << i.adr << "]  ";
    oss << " $" << std::hex << std::setw(4) << std::setfill('0') << std::left << i.ins << std::endl;

    return oss.str();
}

/*
 * initProgram()
 */
void Program::init(void)
{
    this->instructions.erase(this->instructions.begin(), this->instructions.end());
}

/*
 * get()
 */
Instr Program::get(const unsigned int idx) const
{
    if(idx >= 0 && idx < this->instructions.size())
        return this->instructions[idx];

    return Instr();
}

/*
 * getStr()
 */
std::string Program::getStr(const unsigned int idx)
{
    return this->instr_to_string(this->instructions[idx]);
}

void Program::add(const Instr& i)
{
    this->instructions.push_back(i);
}

void Program::writeMem(const uint16_t addr, const uint16_t val)
{
    Instr ins;

    ins.adr = addr;
    ins.ins = val;
    this->instructions.push_back(ins);
}

/*
 * numInstr()
 */
unsigned int Program::numInstr(void) const
{
    return this->instructions.size();
}

/*
 * save()
 */
int Program::save(const std::string& filename)
{
    uint16_t N;
    std::ofstream outfile;

    try {
        outfile.open(filename, std::ios::binary);
    }
    catch(std::ios_base::failure& e) {
        std::cerr << "[" << __FUNCTION__ << "] " << e.what() << std::endl;
        return -1;
    }

    N = (uint16_t) this->instructions.size();
    outfile.write(reinterpret_cast<char*>(&N), sizeof(uint16_t));

    // Debug, remove 
    std::cout << "[" << __FUNCTION__ << "] first address is " 
        << std::hex << std::setw(4) << this->instructions[0].adr 
        << std::endl;

    outfile.write(reinterpret_cast<char*>
                (&this->instructions[0].adr),
                sizeof(uint16_t));
    for(unsigned int idx = 0; idx < this->instructions.size(); ++idx)
    {
        outfile.write(reinterpret_cast<char*>
                (&this->instructions[idx].ins),
                sizeof(uint16_t));
        if(this->verbose)
        {
            std::cout << "Wrote instruction " << idx << "/" 
                << this->instructions.size() << "\r";
        }
    }

    if(this->verbose)
        std::cout << std::endl;
    outfile.close();

    return 0;
}

/*
 * load()
 */
int Program::load(const std::string& filename)
{
    std::ifstream infile;
    uint16_t num_records;
    uint16_t addr;

    this->instructions.clear();

    try {
        infile.open(filename, std::ios_base::binary);
    }
    catch(std::ios_base::failure& e) {
        std::cerr << "[" << __FUNCTION__ << "] " << e.what() << std::endl;
        return -1;
    }

    infile.read(reinterpret_cast<char*>(&num_records), sizeof(uint16_t));
    if(num_records == 0)
    {
        std::cerr << "[" << __FUNCTION__ << "] no records in file " 
            << filename << std::endl;
        return -1;
    }
    // Load the first (only) address pointer 
    infile.read(reinterpret_cast<char*>(&addr), sizeof(uint16_t));

    Instr instr;
    for(unsigned int idx = 0; idx < num_records; ++idx)
    {
        infile.read(reinterpret_cast<char*>(&instr.ins), sizeof(uint16_t));
        //infile.read(reinterpret_cast<char*>(&instr.adr), sizeof(uint16_t));
        instr.adr = addr;
        this->instructions.push_back(instr);
        addr++;
    }
    infile.close();

    return 0;
}

/*
 * writeObj()
 */
int Program::writeObj(const std::string& filename)
{
    std::ofstream outfile;
    uint8_t ub, lb;

    try {
        outfile.open(filename, std::ios_base::binary);
    }
    catch(std::ios_base::failure& e) {
        std::cerr << "[" << __FUNCTION__ << "] " << e.what() << std::endl;
        return -1;
    }

    for(unsigned int i = 0; i < this->instructions.size(); ++i)
    {
        // write address
        ub = (uint8_t) (this->instructions[i].adr >> 8) & 0xFF;
        lb = (uint8_t) (this->instructions[i].adr >> 0) & 0xFF;
        outfile.write(reinterpret_cast<char*>(&ub), sizeof(uint8_t));
        outfile.write(reinterpret_cast<char*>(&lb), sizeof(uint8_t));
        // Write data 
        ub = (uint8_t) (this->instructions[i].ins >> 8) & 0xFF;
        lb = (uint8_t) (this->instructions[i].ins >> 0) & 0xFF;
        outfile.write(reinterpret_cast<char*>(&ub), sizeof(uint8_t));
        outfile.write(reinterpret_cast<char*>(&lb), sizeof(uint8_t));
        if(this->verbose)
        {
            std::cout << "[" << __FUNCTION__ << "] Writing instruction "
                << i << "/" << this->instructions.size() << "\r";
        }
    }
    if(this->verbose)
        std::cout << std::endl << "...done" << std::endl;
    outfile.close();

    return 0;
}

/* 
 * readObj()
 */
int Program::readObj(const std::string& filename)
{
    std::ifstream infile;
    size_t num_bytes;
    uint16_t num_records;
    uint8_t ub, lb;

    try {
        infile.open(filename, std::ios_base::binary);
    }
    catch(std::ios_base::failure& e) {
        std::cerr << "[" << __FUNCTION__ << "] " << e.what() << std::endl;
        return -1;
    }

    // Figure out how long the file is.
    infile.seekg(0, std::ios::end);
    num_bytes = infile.tellg(); 
    if(num_bytes % 4 != 0)
    {
        std::cerr << "[" << __FUNCTION__ << "] contains " 
            << num_bytes << " bytes (" << num_bytes / 4 << " records)"
            << std::endl;
        return -1;
    }
    infile.seekg(0, std::ios::beg);
    num_records = num_bytes / 4;
    this->instructions.clear();

    Instr instr;
    for(unsigned int i = 0; i < num_records; ++i)
    {
        instr.adr = 0;
        instr.ins = 0;
        infile.read(reinterpret_cast<char*>(&ub), sizeof(uint8_t));
        infile.read(reinterpret_cast<char*>(&lb), sizeof(uint8_t));
        instr.adr = (ub << 8) | lb;
        infile.read(reinterpret_cast<char*>(&ub), sizeof(uint8_t));
        infile.read(reinterpret_cast<char*>(&lb), sizeof(uint8_t));
        instr.ins = (ub << 8) | lb;
        this->instructions.push_back(instr);
    }

    infile.close();

    return 0;
}


void Program::setVerbose(const bool v)
{
    this->verbose = v;
}
bool Program::getVerbose(void) const
{
    return this->verbose;
}
