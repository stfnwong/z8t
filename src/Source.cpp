/* SOURCE
 * Intermediate representation of line info for Z80
 *
 * Stefan Wong 2018
 */

#include <iomanip>
#include <iostream>
#include "Source.hpp"

/*
 * Token
 */
Token::Token()
{
    this->val = "\0";
    this->type = SYM_NULL;
}

Token::Token(const std::string& val, const TokenType& t)
{
    this->val  = val;
    this->type = t;
}

bool Token::operator==(const Token& that) const
{
    if(this->val != that.val)
        return false;
    if(this->type != that.type)
        return false;
    return true;
}

bool Token::operator!=(const Token& that) const
{
    return !(*this == that);
}

/*
 * Opcode
 */
Opcode::Opcode() : code(0), mnemonic("\0") {}

Opcode::Opcode(uint16_t code, const std::string& mnemonic)
{
    this->code = code;
    this->mnemonic = mnemonic;
}

bool Opcode::operator==(const Opcode& that) const
{
    if(this->code != that.code)
        return false;
    if(this->mnemonic != that.mnemonic)
        return false;
    
    return true;
}

bool Opcode::operator!=(const Opcode& that) const
{
    return !(*this == that);
}


/* 
 * Symbol
 */
Symbol::Symbol()
{
    this->addr  = 0;
    this->label = "\0";
}

Symbol::Symbol(const uint16_t addr, const std::string& label)
{
    this->addr  = addr;
    this->label = label;
}

bool Symbol::operator==(const Symbol& that) const
{
    if(this->addr != that.addr)
        return false;
    if(this->label != that.label)
        return false;
    return true;
}

bool Symbol::operator!=(const Symbol& that) const
{
    return !(*this == that);
}

/*
 * SymbolTable
 */
SymbolTable::SymbolTable() {} 

void SymbolTable::add(const Symbol& s)
{
    this->syms.push_back(s);
}


/* 
 * InstrTable
 */
InstrTable::InstrTable() {} 

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
 * TextLine
 */
TextLine::TextLine()
{
    this->init();
}

TextLine::TextLine(const TextLine& that)
{
    this->opcode   = that.opcode;
    this->label    = that.label;
    this->symbol   = that.symbol;
    this->line_num = that.line_num;
    this->addr     = that.addr;
}

void TextLine::init(void)
{
    // Init opcode 
    this->opcode.code = 0;
    this->opcode.mnemonic = "\0";
    // Init others 
    this->label    = "\0";
    this->symbol   = "\0";
    this->line_num = 0;
    this->addr     = 0;

    this->is_label = false;
}

std::string TextLine::toString(void)
{
    std::ostringstream oss;

    oss << "---------------------------------------------------------------------" << std::endl;
    oss << "Line  Type   Addr  Mnemonic    Opcode  flags  O1  O2 O3   " << std::endl;
    //oss << "Line  Type   Addr  Mnemonic    Opcode  flags   arg1  arg2  arg3  imm  " << std::endl;

    oss << std::left << std::setw(6) << std::setfill(' ') << this->line_num;
    oss << "[";
    oss << "] ";
    oss << std::right << "0x" << std::hex << std::setw(4) << std::setfill('0') << this->addr << " ";
    oss << std::left << std::setw(12) << std::setfill(' ') << this->opcode.mnemonic;
    oss << "0x" << std::right << std::hex << std::setw(4) << std::setfill('0') << this->opcode.code << "   ";
    // Insert flag chars
    oss << "...";
    // Registers
    oss << "  ";

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


//SourceInfo::SourceInfo(const SourceInfo& that)
//{
//    for(unsigned int idx = 0; idx < that.info.size(); ++idx)
//        this->info.push_back(that.info[idx]);
//}

void SourceInfo::add(const TextLine& l)
{
    this->info.push_back(l);
}

/*
 * get()
 */
TextLine SourceInfo::get(const unsigned int idx) const
{
    if(idx >= 0 && idx < this->info.size())
        return this->info[idx];
    
    TextLine l;
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
