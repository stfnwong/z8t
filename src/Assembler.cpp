/*
 * ASSEMBLER
 * Z80 assembler 
 *
 * Stefan Wong 2018
 */

#include <cctype>
#include <fstream>
#include <iostream>
#include <iomanip>

#include "Common.hpp"
#include "Assembler.hpp"
#include "Instruction.hpp"


/*
 *  ASSEMBLER
 */
Assembler::Assembler()
{
    this->init();
    this->token_buf_size = 128;
    this->init_mem();
}

/*
 * init()
 */
void Assembler::init(void)
{
    this->cur_line       = 1;
    this->cur_pos        = 0;
    this->cur_pos        = 0;
    this->cur_addr       = TEXT_START_ADDR;     
    this->source_info.init();
    this->program.init();
    // init lookups 
    this->instr_lookup     = TokenLookup(TokenSet::Instructions);
    this->register_lookup  = TokenLookup(TokenSet::Registers);
    this->condition_lookup = TokenLookup(TokenSet::Conditions);
}

/*
 * init_mem()
 */
void Assembler::init_mem(void)
{
    this->token_buf = new char [this->token_buf_size];
}

/*
 * exhausted()
 */
bool Assembler::exhausted(void)
{
    return (this->cur_char == '\0') ? true : false;
}

/*
 * advance()
 */
void Assembler::advance(void)
{
    if(this->exhausted())
        return;
    this->cur_pos++;
    this->cur_char = this->source[this->cur_pos];    
    if(this->cur_char == '\n')
        this->cur_line++;
}

/* 
 * is_space()
 */
bool Assembler::is_space(void)
{
    if(this->cur_char == ' ' || this->cur_char == '\n')
        return true;

    return false;
}

/* 
 * is_comment()
 */
bool Assembler::is_comment(void)
{
    return (this->cur_char == ';') ? true : false;
}

/* 
 * skip_whitespace()
 */
void Assembler::skip_whitespace(void)
{
    while(this->cur_char == ' ' || 
          this->cur_char == '\n' || 
          this->cur_char == '\t')
        this->advance();
}

void Assembler::skip_to_next_token(void)
{
    while(this->cur_char != '\n')
    {
        if(std::isalnum(this->cur_char))
            break;                  // this is the start of a new token
        if(this->cur_char == ' ')   // end
            break;
        if(this->cur_char == '\n')
        {
            this->advance();
            break;
        }
        if(this->cur_char == ';')
            break;
        if(this->cur_char == ',')
            break;
        this->advance();
    }
}


/* 
 * skip_line()
 */
void Assembler::skip_line(void)
{
    while(this->cur_char != '\n')
        this->advance();

    this->advance();    // skip over the newline char
}

/*
 * scan_token()
 */
void Assembler::scan_token(void)
{
    unsigned int idx = 0;

    this->skip_whitespace();
    while(idx < (this->token_buf_size-1))
    {
        if(this->cur_char == ' ')   // end
            break;
        if(this->cur_char == '\n')
            break;
        if(this->cur_char == ';')
            break;
        if(this->cur_char == ',')
            break;

        this->token_buf[idx] = std::tolower(this->cur_char);
        this->advance();
        idx++;
    }

    this->token_buf[idx] = '\0';
    if(this->cur_char == ',')   // skip over any seperators
        this->advance();

    if(this->verbose)
    {
        msg_general_funcname(
                __func__,
                this->cur_line, 
                std::string("token_buf contains ") + std::string(this->token_buf)
        );
    }
}

/*
 * check_ahead()
 * Similar to scan token, but doesn't advance the internal position tracking.
 * Useful to see if there is another token or if the line ends.
 */
bool Assembler::check_ahead(void)
{
    unsigned int idx = this->cur_pos;
    char check_char = this->source[idx];

    while(!this->exhausted())
    {
        check_char = this->source[idx];
        if(check_char == '\n')
            break;
        if(check_char == ';')
            break;
        if(check_char == ' ' || check_char == ',')
            idx++;
        if(std::isalnum(check_char))
            return true;
    }

    return false;
}


/*
 * tok_string_to_literal()
 */
Token Assembler::tok_string_to_literal(const std::string& tok_string) const
{
    Token token;

    token.repr = tok_string;
    try
    {
        if(tok_string[0] == '$')
            token.val = std::stoi(tok_string.substr(1, tok_string.size()-1), nullptr, 16);
        else if(tok_string[0] == '%')
            token.val = std::stoi(tok_string.substr(1, tok_string.size()-1), nullptr, 2);
        else
            token.val = std::stoi(tok_string, nullptr, 10);
        token.type = SYM_LITERAL;
    }
    catch(std::exception& ex)
    {
        if(this->verbose)
        {
            std::cerr << "[" << __func__ << "] got exception " << ex.what() 
                << " on line " << std::dec << this->line_info.line_num << std::endl;
            std::cerr << "[" << __func__ << "] returning token " << token.toString() << std::endl;
        }
    }

    return token;
}

/*
 * next_token()
 */
Token Assembler::next_token(void) 
{
    Token token;
    std::string tok_string;
    
    this->scan_token();
    tok_string = std::string(this->token_buf);  

    // check if this is an instruction or directive
    token = this->lookup_instruction(tok_string);
    if(token.type != SYM_NULL)
        return token;
    // check if this is a register 
    token = this->lookup_register(tok_string);
    if(token.type != SYM_NULL)
        return token;
    // check if this is a conditional 
    //token = this->lookup_condition(tok_string);
    //if(token.type != SYM_NULL)
    //    return token;

    // see if this is some kind of literal 
    token = this->parse_literal(tok_string);
    if(token.type != SYM_NULL)
        return token;

    // presume this is a label
    token.val = 0;
    token.repr = tok_string;
    token.type = SYM_LABEL;

    return token;
}

/*
 * parse_literal()
 */
Token Assembler::parse_literal(const std::string& tok_string)
{
    Token token;

    // Check if this is a literal
    if(tok_string[0] == '$' || tok_string[0] == '%' || std::isdigit(tok_string[0]))
    {
        token = this->tok_string_to_literal(tok_string);
    }
    // if the next character is alpha assume this is a labelled indirect 
    else if(tok_string[0] == '(' && std::isalpha(tok_string[1]))
    {
        token.type = SYM_LITERAL_IND;
        token.repr = tok_string.substr(1, tok_string.length() - 2);
    }
    else if(tok_string[0] == '(')
    {
        // This must be a literal indirect (or its invalid) because if it
        // was a register indirect we would have a valid token before we 
        // check if the type was SYM_NULL.
        unsigned int close_idx = 0;         
        while(tok_string[close_idx] != ')' && close_idx < tok_string.size())
            close_idx++;
        if(close_idx >= tok_string.size())
        {
            this->line_info.error = true;
            this->line_info.errstr = "no matching closing paren on line " + std::to_string(this->cur_line);
            return token;
        }
        // we know that tok_string[0] is an opening paren
        std::string tok_substr = tok_string.substr(1, close_idx-1);
        token = this->tok_string_to_literal(tok_substr);
        token.type = SYM_LITERAL_IND;
    } 
    else
    {
        token.val = 0;
        token.repr = tok_string;
        token.type = SYM_LABEL;
    }

    return token;
}

/*
 * lookup_instruction()
 */
Token Assembler::lookup_instruction(const std::string& tok_string)
{
    return this->instr_lookup.lookup(tok_string);
}

/*
 * lookup_register()
 */
Token Assembler::lookup_register(const std::string& tok_string)
{
    return this->register_lookup.lookup(tok_string);
}

/*
 * lookup_condition()
 */
Token Assembler::lookup_condition(const std::string& tok_string)
{
    return this->condition_lookup.lookup(tok_string);
}

/*
 * parse_jump()
 */
void Assembler::parse_jump(void)
{
    // jump instructions have one of two forms 
    //
    // jr cond,  literal
    // OR
    // jr literal

    int arg_num = 0;
    Token token; 

    //token = this->next_token();
    this->scan_token();
    token = this->lookup_condition(std::string(this->token_buf));
    if(token.type == SYM_COND)
    {
        this->line_info.args[arg_num] = token;
        this->scan_token();
        arg_num++;
    }
    // Try and get a literal. We either got the condition
    // already or the condition test failed and we are checking the
    // same token is a literal
    token = this->parse_literal(std::string(this->token_buf));
    if(token.type == SYM_LITERAL || token.type == SYM_LABEL)
    {
        this->line_info.args[arg_num] = token;
        if(token.type == SYM_LABEL)
            this->line_info.sym_arg = arg_num;      // mark for symbol resolve
    }
    else
    {
        this->line_info.error = true;
        this->line_info.errstr = this->line_info.opcode.repr + ": failed to parse " + this->line_info.opcode.repr 
            + ", current token " + token.toString();
    }
}

/*
 * parse_ret()
 */
void Assembler::parse_ret(void)
{
    Token token;

    if(this->check_ahead())
    {
        // the next token should be a conditional
        this->scan_token();
        token = this->lookup_condition(std::string(this->token_buf));
        if(token.type == SYM_COND)
            this->line_info.args[0] = token;
        else
        {
            this->line_info.error = true;
            this->line_info.errstr = "Expected conditional after ret, got " + token.toString()
                + "\n line processed so far : " + this->line_info.toInstrString();
        }
    }
}

/*
 * parse_rst()
 */
void Assembler::parse_rst(void)
{
    Token token;

    // RST can only take some specific values as args
    this->scan_token();
    token = this->parse_literal(std::string(this->token_buf));
    if(token.type == SYM_LITERAL)
    {
        switch(token.val)
        {
            case 0:
            case 0x10:
            case 0x20:
            case 0x30:
            case 0x08:
            case 0x18:
            case 0x28:
            case 0x38:
                this->line_info.args[0] = token;
                break;
            default:
                this->line_info.error = true;
                this->line_info.errstr = "rst given invalid literal value " 
                    + token.repr;
                break;
        }
    }
    else
    {
        this->line_info.error = true;
        this->line_info.errstr = this->line_info.opcode.repr + ": failed to parse " 
            + this->line_info.opcode.repr 
            + ", current token " + token.toString();
    }

}

/*
 * parse_call()
 */
void Assembler::parse_call(void)
{
    int argn = 0;
    Token token;

    this->scan_token();     
    // Try a conditional. If there is one then set the first arg 
    // as the conditional and try to get a literal
    token = this->lookup_condition(std::string(this->token_buf));
    if(token.type == SYM_COND)
    {
        this->line_info.args[argn] = token;
        this->scan_token();
        argn++;
    }
    token = this->parse_literal(std::string(this->token_buf));
    if(token.type == SYM_LITERAL || token.type == SYM_LABEL)
    {
        this->line_info.args[argn] = token;
        if(token.type == SYM_LABEL)
            this->line_info.sym_arg = argn;      // mark for symbol resolve
    }
    else
    {
        this->line_info.error = true;
        this->line_info.errstr = this->line_info.opcode.repr + ": failed to parse " + this->line_info.opcode.repr 
            + ", current token " + token.toString();
    }
}

/*
 * parse_in()
 */
void Assembler::parse_in(void)
{
    Token token;

    // first token must be REG_A 
    this->scan_token();
    token = this->lookup_register(std::string(this->token_buf));

    if(token.val == REG_A)
        this->line_info.args[0] = token;
    else
    {
        this->line_info.error = true;
        this->line_info.errstr = "in expected a as first operand, got " + token.repr;
        return;
    }

    // next token must be a literal indirect
    this->scan_token();
    token = this->parse_literal(std::string(this->token_buf));

    if(token.type == SYM_LITERAL_IND)
        this->line_info.args[1] = token;
    else
    {
        this->line_info.error = true;
        this->line_info.errstr = "in expected (*) as second operand, got " + token.repr;
    }
}
/*
 * parse_out()
 */
void Assembler::parse_out(void)
{
    Token token;

    // next token must be a literal indirect
    this->scan_token();
    token = this->parse_literal(std::string(this->token_buf));

    if(token.type == SYM_LITERAL_IND)
        this->line_info.args[0] = token;
    else
    {
        this->line_info.error = true;
        this->line_info.errstr = "in expected (*) as first operand, got " + token.repr;
        return;
    }

    // next token must be REG_A 
    this->scan_token();
    token = this->lookup_register(std::string(this->token_buf));

    if(token.val == REG_A)
        this->line_info.args[1] = token;
    else
    {
        this->line_info.error = true;
        this->line_info.errstr = "out expected a as second operand, got " + token.repr;
        return;
    }
}

/*
 * parse_one_literal()
 * Parse only a single literal into the first argument slot
 */
void Assembler::parse_one_literal(void)
{
    Token token;

    this->scan_token();
    token = this->parse_literal(std::string(this->token_buf));
    if(token.type == SYM_LITERAL || token.type == SYM_LABEL)
    {
        this->line_info.args[0] = token;
        if(token.type == SYM_LABEL)
            this->line_info.sym_arg = 0;      // mark for symbol resolve
    }
    else
    {
        this->line_info.error = true;
        this->line_info.errstr = this->line_info.opcode.repr + ": failed to parse " + this->line_info.opcode.repr 
            + ", current token " + token.toString();
    }
}


/*
 * parse_one_arg()
 */
void Assembler::parse_arg(int arg_idx)
{
    Token token;

    // token can either be register or literal
    this->scan_token();
    // try and match a register
    token = this->lookup_register(std::string(this->token_buf));
    if(token.type == SYM_REG)
    {
        this->line_info.args[arg_idx] = token;
        return;
    }
        
    // try and match a literal
    token = this->parse_literal(std::string(this->token_buf));
    if(token.needs_lookup())
    {
        this->line_info.sym_arg = arg_idx;
    }
    this->line_info.args[arg_idx] = token;

    if(token.type == SYM_NULL)
    {
        // only add a new error here if there wasn't already one 
        if(!this->line_info.error)
        {
            this->line_info.error = true;
            this->line_info.errstr = "Argument " + std::to_string(arg_idx) + " must be REG, LITERAL, COND, or LABEL (got "
                + std::string(token.toString()) + ")";
        }
        if(this->verbose)
            std::cout << "[" << __func__ << "] " << this->line_info.error << std::endl;
    }
}



/*
 * parse_instruction()
 */
void Assembler::parse_instruction(const Token& token)
{
    // get the corresponding opcode
    this->line_info.opcode = this->lookup_instruction(token.repr);
    if(this->verbose)
    {
        std::cout << "[" << __func__ << "] parsing instruction token " 
            << token.toString() << std::endl;
    }

    // Since this is already a token object we can just
    // jump based on the value
    switch(token.val)
    {
        case INSTR_ADD:
        case INSTR_ADC:
        case INSTR_LD: 
        case INSTR_SBC:
            this->parse_arg(0);
            this->parse_arg(1);
            break;

        case INSTR_JR:
        case INSTR_JP:
            this->parse_jump();
            break;

        // instructions that accept a single argument
        case INSTR_AND: 
        case INSTR_CP:
        case INSTR_DEC:
        case INSTR_INC:
        case INSTR_OR:
        case INSTR_POP:
        case INSTR_PUSH:
        case INSTR_SUB:
        case INSTR_XOR:
            this->parse_arg(0);
            break;

        case INSTR_DJNZ:
            this->parse_one_literal();
            break;

        case INSTR_RET:
            this->parse_ret();
            break;
            
        case INSTR_RST:
            this->parse_rst();
            break;

        case INSTR_CALL:
            this->parse_call();
            break;

        case INSTR_IN:
            this->parse_in();
            break;

        case INSTR_OUT:
            this->parse_out();
            break;

        case INSTR_CCF:
        case INSTR_CPL:
        case INSTR_RRA:
        case INSTR_RRCA:
        case INSTR_NOP:
        case INSTR_RLCA:
        case INSTR_RLA:
        case INSTR_DAA:
        case INSTR_SCF:
            // no args
            break;

        default:
            this->line_info.error = true;
            this->line_info.errstr = "Invalid instruction " + std::string(token.repr);
            if(this->verbose)
            {
                error_general_funcname(__func__, this->cur_line, this->cur_addr, this->line_info.errstr);
            }
    }
}

/*
 * read_to_line_end()
 * Read the remaining non-comment text for a given line
 */
std::string Assembler::read_to_line_end(void)
{
    int str_start_idx = this->cur_pos;
    int str_end_idx = this->cur_pos;

    while(!this->exhausted())
    {
        if(this->cur_char == ';' || this->cur_char == '\n')
            break;
        this->advance();
        str_end_idx++;
    }
    
    std::string line_to_end = this->source.substr(str_start_idx, str_end_idx - str_start_idx);

    return line_to_end;
}

/*
 * parse_directive()
 */
void Assembler::parse_directive(const Token& token)
{
    Token arg_token;

    this->line_info.opcode = token; // TODO: may refactor this member into instr and drop LineType
    this->line_info.type = LineType::DirectiveLine;

    switch(token.val)
    {
        // TODO : this is actually and arbitrary comma seperated list of args....
        // TODO: read the whole line in and see if we need to parse as an expression
        case DIR_DEFB:
        case DIR_DEFW:
        case DIR_EQU:       // this must be a literal
            this->line_info.expr = this->read_to_line_end();
            break;

        case DIR_INCLUDE:
            // TODO:
            break;

        case DIR_ORG:   // updates the current address
            arg_token = this->next_token();
            if(arg_token.type != SYM_LITERAL)
            {
                this->line_info.error = true;
                this->line_info.errstr = "org directive got invalid argument " + std::string(arg_token.repr);
                break;
            }
            this->cur_addr = arg_token.val;
            break;

        case DIR_END:
            // TODO: stop assembling
            break;

        default:
            this->line_info.error = true;
            this->line_info.errstr = "Invalid directive " + std::string(token.repr);
            if(this->verbose)
                error_general(this->cur_line, this->cur_addr, this->line_info.errstr);
            break;
    }
}


/*
 * resolve_labels()
 */
bool Assembler::resolve_labels(void)
{
    int16_t label_addr;
    int16_t target_addr;

    // walk over the sourceinfo and check for labels
    for(unsigned int idx = 0; idx < this->source_info.getNumLines(); ++idx)
    {
        LineInfo cur_line = this->source_info.get(idx);
        if(cur_line.sym_arg >= 0)
        {
            label_addr = this->source_info.getSymAddr(cur_line.args[cur_line.sym_arg].repr);
            if(this->verbose)
            {
                std::cout << "[" << __func__ << "] resolving [" << cur_line.args[cur_line.sym_arg].repr 
                    << "] with address [0x" << std::hex << label_addr << "], on line " << std::dec << cur_line.line_num 
                    << " [0x" << std::hex << cur_line.addr << "]" << std::endl;
            }

            if(label_addr > 0)
            {
                switch(cur_line.opcode.val)
                {
                    // for relative jumps
                    case INSTR_JR:
                        target_addr = label_addr - cur_line.addr;
                        if(target_addr > 0)
                            target_addr -= instr_get_size(cur_line.argHash());      // for forward instructions, we are adding to value after PC increment
                        cur_line.args[cur_line.sym_arg] = Token(SYM_LITERAL, target_addr, std::to_string(target_addr));
                        break;

                    // if we have any literals with no value then lookup the value now
                    case INSTR_LD:
                        if(cur_line.args[cur_line.sym_arg].type == SYM_LABEL)
                            cur_line.args[cur_line.sym_arg] = Token(SYM_LITERAL, label_addr, std::to_string(label_addr));
                        else if(cur_line.args[cur_line.sym_arg].type == SYM_LITERAL_IND)
                        {
                            cur_line.eval(this->source_info);   // sort of untested....
                        }
                        break;

                    // this case should handle absolute jumps (jp
                    // instruction) as well
                    default:
                        cur_line.args[cur_line.sym_arg] = Token(SYM_LITERAL, label_addr, std::to_string(label_addr));
                        break;
                        
                }
            }
            else
            {
                error_general(
                        cur_line.line_num,
                        cur_line.addr,
                        "Failed to resolve symbol " + cur_line.args[cur_line.sym_arg].repr + 
                        " in instruction " + cur_line.toInstrString() 
                );
                return false;
            }
            if(this->verbose)
            {
                msg_general_funcname(
                        __func__, 
                        cur_line.line_num,
                        "updated line to " + cur_line.toString()
                );
            }
            this->source_info.update(idx, cur_line);
        }
    }
    
    return true;
}

/*
 * parse_line()
 */
void Assembler::parse_line(void)
{
    Symbol sym;
    Token token;

    if(this->verbose)
    {
        std::cout << "[" << __func__ << "] parsing line " 
            << std::dec << this->cur_line << " with address 0x" 
            << std::hex << this->cur_addr << std::endl;
    }

    this->line_info.init();
    this->line_info.line_num = this->cur_line;
    token = this->next_token();

    if(token.type == SYM_LABEL)
    {
        if((token.repr.back() == ';') || 
           (token.repr.back() == '#') ||
           (token.repr.back() == ':'))
        {
            sym.label = token.repr.substr(
                    0, token.repr.length() - 1
            );
        }
        else
            sym.label = token.repr;

        // add symbol to table
        sym.addr  = this->cur_addr;
        this->source_info.addSym(sym);
        this->line_info.label = sym.label;

        if(this->verbose)
            msg_general_funcname(__func__, this->cur_line, "added symbol " + sym.toString() + " to table");

        // Scan the next token
        token = this->next_token();
        this->line_info.line_num = this->cur_line;
        this->line_info.is_label = true;
    }

    // handle instructions 
    if(token.type == SYM_INSTR)
    {
        this->parse_instruction(token);
    }
    // handle directives  
    else if(token.type == SYM_DIRECTIVE)
    {
        this->parse_directive(token);
    }
    else
    {
        this->line_info.error = true;
        this->line_info.errstr = "got unexpected token " + token.repr + " on line " + std::to_string(this->cur_line);
    }   
    this->line_info.addr = this->cur_addr;

    if(this->line_info.error)
    {
        error_general(
                this->line_info.line_num, 
                this->line_info.addr,
                this->line_info.errstr
        );
        return;
    }

    if(this->line_info.type == LineType::TextLine)
    {
        this->cur_addr = this->cur_addr + instr_get_size(this->line_info.argHash());
    }
    else
    {
        if(this->line_info.opcode.val == DIR_DEFW || this->line_info.opcode.val == DIR_EQU)
            this->cur_addr = this->cur_addr + 2;
        else if(this->line_info.opcode.val != DIR_ORG)
            this->cur_addr++;
    }
}

/*
 * read()
 */
int Assembler::read(const std::string& filename)
{
    std::ifstream infile;
    std::string line;

    try {
        infile.open(filename);
    }
    catch(std::ios_base::failure& e){
        std::cerr << "[" << __func__ << "] caught exception " 
            << e.what() << " while reading file " << filename << std::endl;
        return -1;
    }
     
    this->filename = filename;
    while(std::getline(infile, line))
    {
        this->source += line;
        this->source.push_back('\n');
    }
    infile.close();
    this->source.push_back('\0');
    this->cur_char = this->source[0];

    return 0;
}

/*
 * loadSource()
 */
void Assembler::loadSource(const std::string& src)
{
    this->source = src;
}

/*
 * getSourceInfo()
 */
const SourceInfo& Assembler::getSourceInfo(void) const
{
    return this->source_info;
}


/*
 * assem_instr()
 */
void Assembler::assem_instr(void)
{
    Instr cur_instr;
    LineInfo line;

    for(unsigned int idx = 0; idx < this->source_info.getNumLines(); ++idx)
    {
        line = this->source_info.get(idx);
        line.eval(this->source_info);       

        if(line.type == LineType::TextLine)
        {
            auto lookup_val = instr_hash_to_code.find(line.argHash());
            if(lookup_val != instr_hash_to_code.end())
            {
                auto instr_size = lookup_val->second;
                cur_instr.size = instr_size.second;
                // TODO: debug, remove 
                //std::cout << "[" << __func__ << "] got instr_size as " << std::dec << cur_instr.size 
                //    << " for instruction " << line.toInstrString() << " at line " << line.line_num 
                //    << " (hash was 0x" << std::hex << line.argHash() << ")" << std::endl;
                if(cur_instr.size == 1)
                    cur_instr.ins  = instr_size.first;
                else if(cur_instr.size == 2 && line.args[0].type == SYM_LITERAL)
                    cur_instr.ins = (instr_size.first << 8) | (line.args[0].val & 0xFF);
                else if(cur_instr.size == 2 && line.args[1].type == SYM_LITERAL)
                    cur_instr.ins = (instr_size.first << 8) | (line.args[1].val & 0xFF);
                // ld bc, ** | ld de, ** | ld hl, ** | ld sp, **
                else if(cur_instr.size == 3 && line.args[1].type == SYM_LITERAL)
                    cur_instr.ins = (instr_size.first << 16) | (line.args[1].val & 0xFFFF);
                // ld (**) hl | ld (**) a
                else if(cur_instr.size == 3 && line.args[0].type == SYM_LITERAL_IND)
                {
                    line.eval(this->source_info);
                    cur_instr.ins = (line.args[0].val & 0xFFFF) | (instr_size.first << 16);
                }
                // ld hl (**) | ld a (**)
                else if(cur_instr.size == 3 && line.args[1].type == SYM_LITERAL_IND)
                {
                    line.eval(this->source_info);
                    cur_instr.ins = (instr_size.first << 16) | (line.args[0].val & 0xFFFF);
                }
                else
                {
                    error_general(
                            line.line_num,
                            line.addr,
                            std::string("failed to assemble instruction " + line.toInstrString())
                    );
                    return;
                }
            }
            else
            {
                error_general(
                        line.line_num, 
                        line.addr, 
                        std::string("invalid instruction " + line.toInstrString())
                );
                return;
            }
            cur_instr.adr = line.addr;      
        }
        else if(line.type == LineType::DirectiveLine)
        {
            if(line.expr.size() > 0 && line.evaluated == false)
                line.eval(this->source_info);

            cur_instr.adr = line.addr;
            if(line.opcode.val == DIR_DEFW || line.opcode.val == DIR_EQU)
            {
                cur_instr.size = 2;
                cur_instr.ins  = uint16_t(line.data);
            }
            else
            {
                cur_instr.size = 1;     
                cur_instr.ins  = uint16_t(line.data);
            }
        }
        else
        {
            error_general(line.line_num, line.addr, std::string("invalid line type " + line.toInstrString()));
            return;
        }
        this->program.add(cur_instr);
    }
}

/*
 * assemble()
 */
void Assembler::assemble(void)
{
    this->init();
    this->cur_char = this->source[0];

    if(this->source.length() == 0)
        return;

    this->cur_char = this->source[0];

    while(!this->exhausted())
    {
        if(this->is_space())
        {
            this->skip_whitespace();
            continue;
        }
        if(this->is_comment())
        {
            this->skip_line();
            continue;
        }
        this->parse_line();
        this->source_info.add(this->line_info);

        // if there were errors then stop on this line
        if(this->line_info.error)
        {
            error_general(
                    this->cur_line,
                    this->cur_addr,
                    this->line_info.errstr
            );
            return;
        }
    }

    // Resolve symbols 
    if(!this->resolve_labels())
        return;
    // now walk over the sourceinfo and assemble
    this->assem_instr();
}

unsigned int Assembler::getCurAddr(void) const
{
    return this->cur_addr;
}

Program Assembler::getProgram(void) const
{
    return this->program;
}

void Assembler::setVerbose(const bool v)
{
    this->verbose = v;
}
bool Assembler::getVerbose(void) const
{
    return this->verbose;
}
