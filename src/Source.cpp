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

bool Token::needs_lookup(void) const
{
    return ((this->repr.length() > 0) && (this->val == -1)) ? true : false;
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
        case SYM_DIRECTIVE:
            return "DIRECTIVE <" + std::string(this->repr) + "> " + std::to_string(this->val);
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

TokenLookup::TokenLookup(const TokenSet& set)
{
    switch(set)
    {
        case TokenSet::Instructions:
            for(const Token& token : Z80_INSTRUCTIONS)
                this->name_to_token[token.repr] = token;
            break;
        case TokenSet::Registers:
            for(const Token& token: Z80_REGISTERS)
                this->name_to_token[token.repr] = token;
            break;
        case TokenSet::Conditions:
            for(const Token& token: Z80_CONDITIONS)
                this->name_to_token[token.repr] = token;
            break;
        default:
            for(const Token& token : Z80_TOKENS)
                this->name_to_token[token.repr] = token;
            break;
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
 * ======== LINE INFO ======== //
 */

LineInfo::LineInfo() 
{
    this->init();
}

LineInfo::LineInfo(const Token& opcode, const Token& arg1, const Token& arg2)
{
    this->init();   // ensure members have known good values 
    this->opcode = opcode;
    this->args[0] = arg1;
    this->args[1] = arg2;
}

void LineInfo::init(void)
{
    // Common fields
    this->type = LineType::TextLine;
    this->label.clear();    
    this->errstr.clear();   
    this->line_num = 0;
    this->addr     = 0;
    this->is_label = false;
    this->error    = false;
    // text fields 
    this->opcode.init();
    this->sym_arg  = -1; 
    for(int i = 0; i < 2; ++i)
        this->args[i].init();
    // directive fields 
    this->eval_result.init();
    this->expr.clear();
    this->data = 0;
    this->evaluated = false;
    //this->data.clear();
}

bool LineInfo::operator==(const LineInfo& that) const
{
    // Common fields 
    if(this->type != that.type)
        return false;
    if(this->label != that.label)
        return false;
    // don't bother comparing the error string
    if(this->line_num != that.line_num)
        return false;
    if(this->addr != that.addr)
        return false;
    if(this->is_label != that.is_label)
        return false;
    if(this->error != that.error)
        return false;
    // text fields
    if(this->opcode != that.opcode)
        return false;
    if(this->label != that.label)
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
    // directive fields
    // we don't bother comparing the directive strings 
    if(this->data != that.data)
        return false;
    //if(this->data_size() != that.data_size())
    //    return false;
    //for(unsigned int i = 0; i < this->data_size(); ++i)
    //{
    //    if(this->data[i] != that.data[i])
    //        return false;
    //}

    return true;
}

bool LineInfo::operator!=(const LineInfo& that) const
{
    return !(*this == that);
}

/*
 * argHash()
 */
uint32_t LineInfo::argHash(void) const
{
    uint32_t hash;

    hash = (this->opcode.val & 0xFF) << 16;

    for(int i = 0; i < 2; ++i)
    {
        if(this->args[i].type == SYM_LITERAL || 
           this->args[i].type == SYM_LITERAL_IND || 
           this->args[i].type == SYM_NULL)
            hash = hash | ((this->args[i].type & 0xFF) << ((1-i) * 8));
        else if(this->args[i].type == SYM_LABEL)
            hash = hash | (SYM_LITERAL << ((1-i) * 8));
        else
            hash = hash | ((this->args[i].val & 0xFF) << ((1-i) * 8));
    }

    return hash;
}


/*
 * LineInfo::eval()
 */
void LineInfo::eval(const SourceInfo& info)
{
    std::string cur_string;
    unsigned int str_start = 0;
    unsigned int str_idx;

    if(this->evaluated)
        return;

    // TODO: not sold on the idea that eval() is cleanest place to handle this conversion....
    // however it is true that we need access to some SourceInfo reference in order to dereference
    // the address of a SYM_LITERAL_IND address...
    if(sym_arg > -1)
    {
        if(this->args[this->sym_arg].type == SYM_LITERAL_IND)
        {
            uint16_t addr = info.getSymAddr(this->args[this->sym_arg].repr);
            if(addr > 0)
            {
                LineInfo dir_line = info.getAddr(addr);
                dir_line.eval(info);        // ensure the target is itself evaluated
                this->args[this->sym_arg] = Token(
                        SYM_LITERAL, 
                        uint16_t(dir_line.data), 
                        std::to_string(dir_line.data)
                );
                this->evaluated = true;
                return;
            }
        }
    }
    if(this->expr.size() == 0)
        return;

    // TODO : note that comma seperated args are not yet supported
    // TODO : check EvalResult and report errors
    for(str_idx = 0; str_idx < this->expr.size(); ++str_idx)
    {
        if(this->expr[str_idx] == ',')
        {
            cur_string = this->expr.substr(str_start, str_idx - str_start);
            str_start = str_idx+1;        // for the next substring
            EvalResult eval = eval_expr_string(cur_string, info);
            this->data = eval.val;
            //this->data.push_back(int(eval));
        }
    }
    // Either there was a string but no substring, or this 
    // is the last substring with no trailing comma
    if(str_idx > 0)
    {
        cur_string = this->expr.substr(str_start, str_idx - str_start);
        EvalResult eval = eval_expr_string(cur_string, info);
        this->data = eval.val;
        //this->data.push_back(int(eval));
    }

    this->evaluated = true;
}

/*
 * LineInfo::size()
 */
unsigned int LineInfo::data_size(void) const
{
    return 1;
    //return this->data.size();
}

/*
 * LineInfo::toString()
 */
std::string LineInfo::toString(void) const
{
    std::ostringstream oss;

    oss << "---------------------------------------------------------------------" << std::endl;
    oss << "Line   Type   Addr   Mnemonic   Opcode  flags  args/data" << std::endl;

    oss << std::left << std::setw(6) << std::setfill(' ') << this->line_num;

    oss << "[";
    if(this->type == LineType::DirectiveLine)
        oss << "D";
    else 
        oss << "T";
    if(this->is_label == true)  // why do I need == true here but nowhere else?
        oss << "l";
    else
        oss << ".";
    if(this->error == true)
        oss << "e";
    else
        oss << ".";
    if(this->evaluated)
        oss << "E";
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
    // Arguments
    oss << "   ";
    if(this->type == LineType::TextLine)
    {
        for(int i = 0; i < 2; ++i)
        {
            if(this->args[i].type == SYM_REG)
                oss << "R";
            else if(this->args[i].type == SYM_COND)
                oss << "C";
            oss << this->args[i].repr << " ";
        }
    }
    else
        oss << "0x" << std::hex << std::setw(2) << std::setfill('0') << this->data;

    // (Next line) Text 
    oss << std::endl;
    oss << "Label [" << std::left << std::setw(16) << std::setfill(' ') << this->label << "] ";
    oss << "Expr [" << std::left << std::setw(16) << std::setfill(' ') << this->expr << "] ";
    //oss << "Symbol[" << std::left << std::setw(16) << std::setfill(' ') << this->symbol << "] ";

    oss << std::endl;
    if(this->errstr.size() > 0)
        oss << this->errstr << std::endl;

    return oss.str();
}


/*
 * diff()
 */
std::string LineInfo::diff(const LineInfo& that)
{
    std::ostringstream oss;

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
    if(this->data != that.data)
    {
        oss << "data [" << this->data
            << "] does not match [" << that.data
            << "]" << std::endl;
    }

    return oss.str();
}

/*
 * toInstrString()
 */
std::string LineInfo::toInstrString(void) const
{
    std::ostringstream oss;

    oss << this->opcode.repr << " " << this->args[0].repr;
    if(this->args[1].type != SYM_NULL)
        oss << "," << this->args[1].repr;

    return oss.str();
}


/*
 * ======== SOURCE INFO ======== //
 */

/*
 * SourceInfo
 */
SourceInfo::SourceInfo()
{
    this->init();
}

/*
 * SourceInfo::init()
 */
void SourceInfo::init(void)
{
    this->info.clear();
}

/*
 * SourceInfo::add()
 */
void SourceInfo::add(const LineInfo& l)
{
    this->info.push_back(l);
    // add directive addresses to lookup table
    if(l.type == LineType::DirectiveLine)
    {
        this->directive_addr_lut.insert(
                {l.addr, this->info.size()-1}
        );
    }
}

/*
 * SourceInfo::hasError()
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
 * SourceInfo::get()
 */
LineInfo SourceInfo::get(const unsigned int idx) const
{
    if(idx >= 0 && idx < this->info.size())
        return this->info[idx];
    
    return LineInfo();
}

/*
 * SourceInfo::getAddr()
 */
LineInfo SourceInfo::getAddr(const int16_t addr) const
{
    auto line = this->directive_addr_lut.find(addr);
    if(line != this->directive_addr_lut.end())
        return this->info[line->second];

    return LineInfo();
}

/*
 * SourceInfo::update()
 */
void SourceInfo::update(const unsigned int idx, const LineInfo& l)
{
    this->info[idx] = l;
}

/*
 * SourceInfo::getNumLines()
 */
unsigned int SourceInfo::getNumLines(void) const
{
    return this->info.size();
}

void SourceInfo::toFile(const std::string& filename) const
{
    std::cout << "[" << __func__ << "] : TODO:" << std::endl;
}


// Symbol table methods 

/*
 * SourceInfo::add()
 */
void SourceInfo::addSym(const Symbol& s)
{
    this->syms.push_back(s);
}

/*
 * SourceInfo::update()
 */
void SourceInfo::updateSym(const unsigned int idx, const Symbol& s)
{
    if(idx < this->syms.size())
        this->syms[idx] = s;
}

/*
 * SourceInfo::get()
 */
Symbol SourceInfo::getSym(const unsigned int idx) const
{
    return this->syms[idx];
}

/*
 * SourceInfo::getAddr()
 */
uint16_t SourceInfo::getSymAddr(const std::string& sym) const
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

std::string SourceInfo::getSymName(const uint16_t addr)  const
{
    for(unsigned int idx = 0; idx < this->syms.size(); ++idx)
    {
        if(addr == this->syms[idx].addr)
            return this->syms[idx].label;
    }

    return "";
}

unsigned int SourceInfo::getNumSyms(void) const
{
    return this->syms.size();
}

/*
 * symTableString()
 */
std::string SourceInfo::symTableString(void) const
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
 * SourceInfo::toString()
 */
std::string SourceInfo::toString(void) const
{
    std::ostringstream oss;

    for(unsigned int idx = 0; idx < this->info.size(); ++idx)
        oss << this->info[idx].toString() << std::endl;

    return oss.str();
}
