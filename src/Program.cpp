/*
 * PROGRAM
 * Binary utils 
 */

#include <fstream>
#include <sstream>
#include <iomanip>
#include <iostream>

#include "Instruction.hpp"
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
        << this->adr << "] 0x" << std::setw(2 * this->size) << this->ins << " (" 
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

// operators 
bool Program::operator==(const Program& that) const
{
    if(this->instructions.size() != that.instructions.size())
        return false;
    for(unsigned int idx = 0; idx < this->instructions.size(); ++idx)
    {
        if(this->instructions[idx] != that.instructions[idx])
            return false;
    }

    return true;
}

bool Program::operator!=(const Program& that) const
{
    return !(*this == that);
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
 * length()
 */
unsigned int Program::length(void) const
{
    return this->instructions.size();
}

/*
 * numBytes()
 */
unsigned int Program::numBytes(void) const
{
    unsigned int num_bytes = 0;
    for(unsigned int idx = 0; idx < this->instructions.size(); ++idx)
        num_bytes += this->instructions[idx].size;

    return num_bytes;
}

/*
 * save()
 */
int Program::save(const std::string& filename)
{
    std::ofstream outfile;

    try {
        outfile.open(filename, std::ios::binary);
    }
    catch(std::ios_base::failure& e) {
        std::cerr << "[" << __func__ << "] " << e.what() << std::endl;
        return -1;
    }

    for(unsigned int idx = 0; idx < this->instructions.size(); ++idx)
    {
        outfile.write(reinterpret_cast<char*>
                (&this->instructions[idx].ins),
                this->instructions[idx].size * sizeof(uint8_t));
        if(this->verbose)
        {
            std::cout << "Wrote instruction " << idx + 1 << "/" 
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
 * Read and decode each byte in the stream. Use the code_to_instr_repr
 * table to work out the instruction size and advance the file pointer
 * correctly.
 */
int Program::load(const std::string& filename)
{
    std::ifstream infile;

    this->instructions.clear();

    try {
        infile.open(filename, std::ios_base::binary);
    }
    catch(std::ios_base::failure& e) {
        std::cerr << "[" << __func__ << "] " << e.what() << std::endl;
        return -1;
    }

    infile.seekg(0, infile.end);

    int idx = 0;
    int length = infile.tellg();
    infile.seekg(0, infile.beg);

    if(this->verbose)
        std::cout << "[" << __func__ << "] file length is " << length << std::endl;

    Instr instr;
    while(idx < length)
    {
        uint8_t opcode;
        uint32_t buf;

        infile.read(reinterpret_cast<char*>(&opcode), sizeof(uint8_t));

        if(this->verbose)
        {
            std::cout << "[" << __func__ << "] read opcode 0x" << std::hex 
                << unsigned(opcode) << std::endl;
        }
        
        auto instr_lookup = code_to_instr_repr.find(opcode);
        if(instr_lookup != code_to_instr_repr.end())
        {
            instr.size = instr_lookup->second.second;

            if(this->verbose)
                std::cout << "[" << __func__ << "] size: " << unsigned(instr.size) << " bytes" << std::endl;

            // TODO : better to read into a single unit32_t?
            if(instr.size == 1)
                instr.ins = opcode;
            else if(instr.size == 2)
            {
                infile.read(reinterpret_cast<char*>(&buf), sizeof(uint8_t));
                instr.ins = buf | (opcode << 8);
            }
            else if(instr.size == 3)
            {
                infile.read(reinterpret_cast<char*>(&buf), 2 * sizeof(uint8_t));
                instr.ins = buf | (opcode << 16);
            }
        }
        if(this->verbose)
            std::cout << "[" << __func__ << "] read instr as " << instr.toString() << std::endl;
        instr.adr = TEXT_START_ADDR + idx;
        idx += instr.size;

        this->instructions.push_back(instr);
        instr.init();
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
        std::cerr << "[" << __func__ << "] " << e.what() << std::endl;
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
            std::cout << "[" << __func__ << "] Writing instruction "
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
        std::cerr << "[" << __func__ << "] " << e.what() << std::endl;
        return -1;
    }

    // Figure out how long the file is.
    infile.seekg(0, std::ios::end);
    num_bytes = infile.tellg(); 
    if(num_bytes % 4 != 0)
    {
        std::cerr << "[" << __func__ << "] contains " 
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

/*
 * toArray()
 */
std::vector<uint8_t> Program::toArray(void) const
{
    std::vector<uint8_t> array;

    // Do some things to get the endianness right. We want the output
    // vector to be in "stream-order".
    for(unsigned int idx = 0; idx < this->instructions.size(); ++idx)
    {
        Instr cur_instr = this->instructions[idx];
        uint32_t mask = 0xFF << (8 * (cur_instr.size-1));

        for(int byte = cur_instr.size; byte > 0; --byte)
        {
            uint8_t cur_byte = (cur_instr.ins & mask) >> ((byte-1) * 8);
            array.push_back(cur_byte);
            mask = mask >> 8;
        }
    }

    return array;
}

/*
 * toString()
 */
std::string Program::toString(void) const
{
    std::ostringstream oss;

    oss << "Program contains " << std::dec << this->instructions.size()
        << " instructions" << std::endl;

    for(unsigned int idx = 0; idx < this->instructions.size(); ++idx)
    {
        oss << this->instructions[idx].toString() << std::endl;
    }

    return oss.str();
}


void Program::setVerbose(const bool v)
{
    this->verbose = v;
}
bool Program::getVerbose(void) const
{
    return this->verbose;
}
