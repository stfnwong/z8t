/* SOURCE
 * Intermediate representation of line info for Z80
 *
 * Stefan Wong 2018
 */

#include <iomanip>
#include <iostream>
#include "Source.hpp"
#include "Expression.hpp"

/*
 * ======== TOKEN ======== //
 */

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
            return "EOF <" + std::string(this->repr) + "> " + std::to_string(this->val);
        case SYM_INSTR:
            return "INSTR <" + std::string(this->repr) + "> " + std::to_string(this->val);
        case SYM_LITERAL:
            return "LITERAL <" + std::string(this->repr) + "> " + std::to_string(this->val);
        case SYM_LITERAL_IND:
            return "LITERAL_IND <" + std::string(this->repr) + "> " + std::to_string(this->val);
        case SYM_LABEL:
            return "LABEL <" + std::string(this->repr) + "> " + std::to_string(this->val);
        case SYM_REG:
            return "REGISTER <" + std::string(this->repr) + "> " + std::to_string(this->val);
        case SYM_COND:
            return "CONDITION <" + std::string(this->repr) + "> " + std::to_string(this->val);
        default:
            return "NULL <" + std::string(this->repr) + "> " + std::to_string(this->val);
    }
}


/*
 * TOKEN LOOKUP
 */
TokenLookup::TokenLookup()
{
    for(const Token& token : Z80_TOKENS)
        this->name_to_token[token.repr] = token;
}


Token TokenLookup::lookup(const std::string& s) const
{
    auto token = this->name_to_token.find(s);
    if(token != this->name_to_token.end())
        return token->second;

    return Token();     // can't find anything, return an empty token
}


/*
 * Opcode lookup
 */
OpcodeLookup::OpcodeLookup()
{
    for(const Token& opcode : Z80_TOKENS)
    {
        if(opcode.type == SYM_INSTR)
        {
            this->val_to_opcode[opcode.val] = opcode;
            this->name_to_opcode[opcode.repr] = opcode;
        }
    }
}

/*
 * get()
 * Get opcode by val
 */
Token OpcodeLookup::get(const int val) const
{
    auto op = this->val_to_opcode.find(val);
    if(op != this->val_to_opcode.end())
        return op->second;

    return Token();     // can't find anything, return an empty token
}


/*
 * get()
 * Get opcode by name
 */
Token OpcodeLookup::get(const std::string& name) const
{
    auto op = this->name_to_opcode.find(name);
    if(op != this->name_to_opcode.end())
        return op->second;

    return Token();     // can't find anything, return an empty token
}

/*
 * ======== SYMBOL ======== //
 */

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

void Symbol::init(void)
{
    this->addr = 0;
    this->label.clear();
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

std::string Symbol::toString(void) const
{
    std::ostringstream oss;

    oss << this->label << " " << std::hex << this->addr;

    return oss.str();
}

/*
 * ======== SYMBOL TABLE ======== //
 */

/*
 * SymbolTable
 */
SymbolTable::SymbolTable() {} 

/*
 * add()
 */
void SymbolTable::add(const Symbol& s)
{
    this->syms.push_back(s);
}

/*
 * update()
 */
void SymbolTable::update(const unsigned int idx, const Symbol& s)
{
    if(idx < this->syms.size())
        this->syms[idx] = s;
}

/*
 * get()
 */
Symbol SymbolTable::get(const unsigned int idx) const
{
    return this->syms[idx];
}

/*
 * getAddr()
 */
uint16_t SymbolTable::getAddr(const std::string& sym) const
{
    uint16_t addr = 0;

    // if this turns out to be a bottleneck then this can be made 
    // and unordered map of labels to addresses
    for(unsigned int idx = 0; idx < this->syms.size(); ++idx)
    {
        if(sym == this->syms[idx].label)
            return this->syms[idx].addr;
    }

    return addr;
}

std::string SymbolTable::getName(const uint16_t addr)  const
{
    for(unsigned int idx = 0; idx < this->syms.size(); ++idx)
    {
        if(addr == this->syms[idx].addr)
            return this->syms[idx].label;
    }

    return "";
}

/*
 * init()
 */
void SymbolTable::init(void)
{
    this->syms.clear();
}

unsigned int SymbolTable::size(void) const
{
    return this->syms.size();
}

/*
 * toString()
 */
std::string SymbolTable::toString(void) const
{
    std::ostringstream oss;

    oss << "Symbol Table: (" << this->syms.size() << " symbols)" << std::endl;
    for(unsigned int idx = 0; idx < this->syms.size(); ++idx)
    {
        oss << "    [" << this->syms[idx].label << "] -> 0x" << std::hex 
            << std::setw(4) << std::setfill('0') << this->syms[idx].addr 
            << std::endl;
    }

    return oss.str();
}

/*
 * ======== TEXT LINE ======== //
 */

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
    this->errstr   = that.errstr;
    this->symbol   = that.symbol;
    this->line_num = that.line_num;
    this->addr     = that.addr;
    this->sym_arg  = that.sym_arg;

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
    this->sym_arg  = -1; 

    this->is_label = false;
    this->error    = false;

    for(int i = 0; i < 2; ++i)
        this->args[i].init();

    this->data.clear();
}

/*
 * argHash()
 */
uint32_t TextLine::argHash(void) const
{
    uint32_t hash = 0;

    if(this->args[0].type == SYM_LITERAL || this->args[0].type == SYM_LABEL)
    {
        hash = ((this->opcode.val  & 0xFF) << 16) | 
               ((SYM_LITERAL & 0xFF) << 8);

        if(this->args[1].val >= 0)
            hash = hash | ((this->args[1].val & 0xFF));
    }
    else if(this->args[1].type == SYM_LITERAL)
    {
        hash = ((this->opcode.val  & 0xFF) << 16) | 
               ((this->args[0].val & 0xFF) << 8) | 
               ((this->args[1].type & 0xFF));
    }
    // this handles the case where the labels have not yet been resolved
    else if(this->args[1].type == SYM_LABEL)
    {
        hash = ((this->opcode.val  & 0xFF) << 16) | 
               ((this->args[0].val & 0xFF) << 8) | 
               ((SYM_LITERAL & 0xFF));
    }
    else
    {
        hash = ((this->opcode.val  & 0xFF) << 16) | 
               ((this->args[0].val & 0xFF) << 8);

        if(this->args[1].val >= 0)
            hash = hash | ((this->args[1].val & 0xFF));
    }

    return hash;
}

/*
 * toString()
 */
std::string TextLine::toString(void) const
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
    // address
    oss << std::right << "0x" << std::hex << std::setw(4) << std::setfill('0') << this->addr << " ";
    // mnemonic
    oss << std::left << std::setw(12) << std::setfill(' ') << this->opcode.repr;
    // opcode
    oss << "0x" << std::right << std::hex << std::setw(4) << std::setfill('0') << this->opcode.val << "   ";
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


/*
 * diff()
 */
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
 * toInstrString()
 */
std::string TextLine::toInstrString(void) const
{
    std::ostringstream oss;

    oss << this->opcode.repr << " " << this->args[0].repr << "," << this->args[1].repr;

    return oss.str();
}

/*
 * ======== DIRECTIVE LINE ======== //
 */
DirectiveLine::DirectiveLine() : expr("") {}

DirectiveLine::DirectiveLine(const std::string& e, const std::vector<int>& v) : expr(e), data(v) {} 

/*
 * DirectiveLine::init()
 */
void DirectiveLine::init(void)
{
    this->expr.clear();
    this->data.clear();
}

/*
 * DirectiveLine::eval()
 */
void DirectiveLine::eval(void)
{
    std::string cur_string;
    unsigned int str_start = 0;
    unsigned int str_idx;

    for(str_idx = 0; str_idx < this->expr.size(); ++str_idx)
    {
        if(this->expr[str_idx] == ',')
        {
            cur_string = this->expr.substr(str_start, str_idx - str_start);
            str_start = str_idx+1;        // for the next substring
            float eval = eval_expr_string(cur_string);
            this->data.push_back(int(eval));
        }
    }
    // Either there was a string but no substring, or this 
    // is the last substring with no trailing comma
    if(str_idx > 0)
    {
        cur_string = this->expr.substr(str_start, str_idx - str_start);
        float eval = eval_expr_string(cur_string);
        this->data.push_back(int(eval));
    }
}

/*
 * DirectiveLine::size()
 */
unsigned int DirectiveLine::size(void) const
{
    return this->data.size();
}

bool DirectiveLine::operator==(const DirectiveLine& that) const
{
    if(this->expr != that.expr)
        return false;
    if(this->size() != that.size())
        return false;
    for(unsigned int i = 0; i < this->size(); ++i)
    {
        if(this->data[i] != that.data[i])
            return false;
    }
    
    return true;
}

bool DirectiveLine::operator!=(const DirectiveLine& that) const
{
    return !(*this == that);
}

/*
 * ======== SOURCE INFO ======== //
 */

/*
 * SourceInfo
 */

SourceInfo::SourceInfo() {} 

/*
 * init()
 */
void SourceInfo::init(void)
{
    this->info.clear();
}

/*
 * add()
 */
void SourceInfo::add(const TextLine& l)
{
    this->info.push_back(l);
}

/*
 * hasError()
 */
bool SourceInfo::hasError(void) const
{
    // if it turns out to be a bottleneck then we can optimize this by
    // not checking each line 
    for(unsigned int idx = 0; idx < this->info.size(); ++idx)
    {
        if(this->info[idx].error)
            return true;
    }

    return false;
}


/*
 * get()
 */
TextLine SourceInfo::get(const unsigned int idx) const
{
    if(idx >= 0 && idx < this->info.size())
        return this->info[idx];
    
    return TextLine();
}

/*
 * update()
 */
void SourceInfo::update(const unsigned int idx, const TextLine& l)
{
    this->info[idx] = l;
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
