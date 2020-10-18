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
Token::Token() : type(SYM_NULL), val(-1), repr("\0") {}

Token::Token(const TokenType& t, int val, const std::string& repr)
{
    this->type = t;
    this->val  = val;
    this->repr = repr;
}
/*
 * copy ctor
 */
Token::Token(const Token& that)
{
    this->type = that.type;
    this->val = that.val;
    this->repr = that.repr;
}

/*
 * ==
 */
bool Token::operator==(const Token& that) const
{
    if(this->type != that.type)
        return false;
    if(this->val != that.val)
        return false;
    if(this->repr != that.repr)
        return false;
    return true;
}

/*
 * !=
 */
bool Token::operator!=(const Token& that) const
{
    return !(*this == that);
}

void Token::init(void)
{
    this->type = SYM_NULL;
    this->val = -1;
    this->repr.clear();
}

/*
 * toString()
 */
std::string Token::toString(void) const
{
    switch(this->type)
    {
        case SYM_EOF:
            return "EOF <" + std::string(this->repr) + ">";
        case SYM_INSTR:
            return "INSTR <" + std::string(this->repr) + ">";
        case SYM_LITERAL:
            return "LITERAL <" + std::string(this->repr) + ">";
        case SYM_LABEL:
            return "LABEL <" + std::string(this->repr) + ">";
        case SYM_REG:
            return "REGISTER <" + std::string(this->repr) + ">";
        case SYM_COND:
            return "CONDITION <" + std::string(this->repr) + ">";
        default:
            return "NULL <" + std::string(this->repr) + ">";
    }
}


/*
 * TOKEN LOOKUP
 */
TokenLookup::TokenLookup()
{
    for(const Token& token : Z80_TOKENS)
    {
        this->name_to_token[token.repr] = token;
    }
}


Token TokenLookup::lookup(const std::string& s) const
{
    auto token = this->name_to_token.find(s);
    if(token != this->name_to_token.end())
        return token->second;

    return Token();     // can't find anything, return an empty token
}


/*
 * Opcode
 */
Opcode::Opcode() : code(-1), mnemonic("\0") {}

Opcode::Opcode(int code, const std::string& mnemonic)
{
    this->code = code;
    this->mnemonic = mnemonic;
}

void Opcode::init(void)
{
    this->code = -1;
    this->mnemonic.clear();
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

std::string Opcode::toString(void) const
{
    std::ostringstream oss;

    oss << this->mnemonic << " [0x" << std::hex << std::setw(4)
        << std::setfill('0') << this->code << "]";

    return oss.str();
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
 * TextLine
 */
TextLine::TextLine()
{
    this->init();
}

/*
 * copy ctor
 */
TextLine::TextLine(const TextLine& that)
{
    this->opcode   = that.opcode;
    this->label    = that.label;
    this->symbol   = that.symbol;
    this->line_num = that.line_num;
    this->addr     = that.addr;

    for(int i = 0; i < 2; ++i)
        this->args[i] = that.args[i];
}

/*
 * ==
 */
bool TextLine::operator==(const TextLine& that) const
{
    if(this->opcode != that.opcode)
        return false;
    if(this->label != that.label)
        return false;
    if(this->symbol != that.symbol)
        return false;
    if(this->line_num != that.line_num)
        return false;
    if(this->addr != that.addr)
        return false;
    for(int i = 0; i < 2; ++i)
    {
        if(this->args[i] != that.args[i])
            return false;
    }

    return true;
}

/*
 * !=
 */
bool TextLine::operator!=(const TextLine& that) const
{
    return !(*this == that);
}

/*
 * init()
 */
void TextLine::init(void)
{
    // Init opcode 
    this->opcode.init();
    // Init others 
    this->symbol.clear();   
    this->label.clear();    
    this->errstr.clear();   
    this->line_num = 0;
    this->addr     = 0;

    this->is_label = false;
    this->error    = false;

    for(int i = 0; i < 2; ++i)
        this->args[i].init();
}

/*
 * toString()
 */
std::string TextLine::toString(void)
{
    std::ostringstream oss;

    oss << "---------------------------------------------------------------------" << std::endl;
    oss << "Line  Type   Addr  Mnemonic    Opcode  flags  args" << std::endl;

    oss << std::left << std::setw(6) << std::setfill(' ') << this->line_num;
    oss << "[";
    if(this->is_label == true)  // why do I need == true here but nowhere else?
        oss << "l";
    else
        oss << ".";
    if(this->error == true)
        oss << "e";
    else
        oss << ".";
    oss << "]  ";
    oss << std::right << "0x" << std::hex << std::setw(4) << std::setfill('0') << this->addr << " ";
    oss << std::left << std::setw(12) << std::setfill(' ') << this->opcode.mnemonic;
    oss << "0x" << std::right << std::hex << std::setw(4) << std::setfill('0') << this->opcode.code << "   ";
    // Insert flag chars
    oss << "...";
    // Registers
    oss << "   ";
    for(int i = 0; i < 2; ++i)
        oss << this->args[i].repr << " ";

    // (Next line) Text 
    oss << std::endl;
    oss << "Label [" << std::left << std::setw(16) << std::setfill(' ') << this->label << "] ";
    oss << "Symbol[" << std::left << std::setw(16) << std::setfill(' ') << this->symbol << "] ";

    oss << std::endl;
    if(this->errstr.size() > 0)
        oss << this->errstr << std::endl;

    return oss.str();
}

std::string TextLine::diff(const TextLine& that)
{
    std::ostringstream oss;

    if(this->symbol != that.symbol)
    {
        oss << "symbol [" << this->symbol
            << "] does not match [" << that.symbol
            << "]" << std::endl;
    }
    if(this->label != that.label)
    {
        oss << "label [" << this->label
            << "] does not match [" << that.label 
            << "]" << std::endl;
    }
    if(this->errstr != that.errstr)
    {
        oss << "errstr [" << this->errstr
            << "] does not match [" << that.errstr
            << "]" << std::endl;
    }
    if(this->opcode != that.opcode)
    {
        oss << "opcode [" << this->opcode.toString()
            << "] does not match [" << that.opcode.toString()
            << "]" << std::endl;
    }

    for(int i = 0; i < 2; ++i)
    {
        if(this->args[i] != that.args[i])
        {
            oss << "arg " << i << " [" << this->args[i].toString() 
                << "] does not match [" << that.args[i].toString()
                << "]" << std::endl;
        }
    }
    if(this->line_num != that.line_num)
    {
        oss << "line num [" << this->line_num
            << "] does not match [" << that.line_num
            << "]" << std::endl;
    }
    if(this->addr != that.addr)
    {
        oss << "addr [" << this->addr
            << "] does not match [" << that.addr
            << "]" << std::endl;
    }
    if(this->is_label != that.is_label)
    {
        oss << "is_label does not match" << std::endl;
    }
    if(this->error != that.error)
    {
        oss << "error does not match" << std::endl;
    }

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
