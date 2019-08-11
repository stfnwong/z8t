/* SOURCE
 * Intermediate representation of line info for Z80
 *
 * Stefan Wong 2018
 */

#include <iomanip>
#include <iostream>
#include "Source.hpp"

/*
 * SymbolTable
 */
SymbolTable::SymbolTable() {} 
SymbolTable::~SymbolTable() {} 

SymbolTable::SymbolTable(const SymbolTable& that)
{
    
}

void SymbolTable::add(const Symbol& s)
{
    this->syms.push_back(s);
}


/* 
 * InstrTable
 */
InstrTable::InstrTable() {} 

InstrTable::~InstrTable() {} 

void InstrTable::init(void)
{
    this->instrs.clear();
}

void InstrTable::add(const Opcode& op)
{
    this->instrs.push_back(op);
}

/*
 * get()
 */
void InstrTable::get(const std::string& name, Opcode& op) const
{
    for(unsigned int idx = 0; idx < this->instrs.size(); ++idx)
    {
        if(this->instrs[idx].mnemonic == name)
        {
            op.code = this->instrs[idx].code;
            op.mnemonic = this->instrs[idx].mnemonic;
            return;
        }
    }
    
    // Can't find any match
    op.code = 0;
    op.mnemonic = "\0";
}

/*
 * get()
 */
void InstrTable::get(const uint16_t code, Opcode& op) const
{
    for(unsigned int idx = 0; idx < this->instrs.size(); ++idx)
    {
        if(this->instrs[idx].code == code)
        {
            op.code = this->instrs[idx].code;
            op.mnemonic = this->instrs[idx].mnemonic;
            return;
        }
    }
    
    // Can't find any match
    op.code = 0;
    op.mnemonic = "\0";
}

/* 
 * getMnemonic()
 */
std::string InstrTable::getMnemonic(const uint16_t code) const
{
    for(unsigned int idx = 0; idx < this->instrs.size(); ++idx)
    {
        if(this->instrs[idx].code == code)
            return this->instrs[idx].mnemonic;
    }
    return "\0";
}




/*
 * LineInfo
 */
LineInfo::LineInfo()
{
    this->init();
}

LineInfo::~LineInfo() {}

LineInfo::LineInfo(const LineInfo& that)
{
    this->line_num = that.line_num;
    this->addr     = that.addr;
}

void LineInfo::init(void)
{
    // Init opcode 
    this->opcode.code = 0;
    this->opcode.mnemonic = "\0";
    // Init others 
    this->label = "\0";
    this->symbol = "\0";
    this->line_num = 0;
    this->addr = 0;

    this->is_label = false;
}

std::string LineInfo::toString(void)
{
    std::ostringstream oss;

    oss << "---------------------------------------------------------------------" << std::endl;
    oss << "Line  Type   Addr  Mnemonic    Opcode  flags  O1  O2 O3   " << std::endl;
    //oss << "Line  Type   Addr  Mnemonic    Opcode  flags   arg1  arg2  arg3  imm  " << std::endl;

    oss << std::left << std::setw(6) << std::setfill(' ') << this->line_num;
    oss << "[";
    //if(this->is_label)
    //    oss << "l";
    //else
    //    oss << ".";
    //if(this->is_directive)
    //    oss << "d";
    //else
    //    oss << ".";
    //if(this->is_imm)
    //    oss << "i";
    //else
    //    oss << ".";
    oss << "] ";
    oss << std::right << "0x" << std::hex << std::setw(4) << std::setfill('0') << this->addr << " ";
    oss << std::left << std::setw(12) << std::setfill(' ') << this->opcode.mnemonic;
    oss << "0x" << std::right << std::hex << std::setw(4) << std::setfill('0') << this->opcode.code << "   ";
    // Insert flag chars
    oss << "...";
    // Registers
    oss << "  ";
    //if(l.reg_flags & LEX_IREG)
    //    oss << "  I"; 
    //else if(l.reg_flags & LEX_BREG)
    //    oss << "  B";
    //else if(l.reg_flags & LEX_FREG)
    //    oss << "  F";
    //else if(l.reg_flags & LEX_KREG)
    //    oss << "  K";
    //else if(l.reg_flags & LEX_DTREG)
    //    oss << " DT";
    //else if(l.reg_flags & LEX_STREG)
    //    oss << " ST";
    //else if(l.reg_flags & LEX_IST)
    //    oss << " [I]";
    //else
    //    oss << " V" << std::right << std::hex << std::setw(1) << l.vx;
    //if(l.reg_flags & LEX_ILD)
    //    oss << " [I]";
    //else
    //    oss << "  V" << std::right << std::hex << std::setw(1) << l.vy;
    //oss << " 0x" << std::right << std::hex << std::setw(2) << std::setfill('0') << l.kk;
    //oss << " 0x" << std::right << std::hex << std::setw(3) << std::setfill('0') << l.nnn;

    // (Next line) Text 
    oss << std::endl;
    oss << "Label [" << std::left << std::setw(16) << std::setfill(' ') << this->label << "] ";
    oss << "Symbol[" << std::left << std::setw(16) << std::setfill(' ') << this->symbol << "] ";

    oss << std::endl;


    return oss.str();
}

/*
 * SourceInfo
 */

SourceInfo::SourceInfo() {} 

SourceInfo::~SourceInfo() {} 

SourceInfo::SourceInfo(const SourceInfo& that)
{
    for(unsigned int idx = 0; idx < that.info.size(); ++idx)
        this->info.push_back(that.info[idx]);
}

void SourceInfo::add(const LineInfo& l)
{
    this->info.push_back(l);
}

/*
 * get()
 */
LineInfo SourceInfo::get(const unsigned int idx) const
{
    if(idx >= 0 && idx < this->info.size())
        return this->info[idx];
    
    LineInfo l;
    return l;
}

/*
 * getNumLines()
 */
unsigned int SourceInfo::getNumLines(void) const
{
    return this->info.size();
}

void SourceInfo::toFile(const std::string& filename) const
{
    std::cout << "[" << __FUNCTION__ << "] : TODO:" << std::endl;
}
