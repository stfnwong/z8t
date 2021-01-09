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

#include "Assembler.hpp"

/*
 * instr_get_size()
 */
uint8_t instr_get_size(uint32_t arg_hash)
{
    auto lookup_val = instr_lookup.find(arg_hash);
    if(lookup_val != instr_lookup.end())
        return lookup_val->second.second;

    return 0;
}

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
    //this->cur_char       = '\0';
    this->source_info.init();
    this->program.init();
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
        std::cout << "[" << __func__ << "] (line " << std::dec 
            << this->cur_line << ") : token_buf contains [" 
            << std::string(this->token_buf) << "] " << std::endl;
    }
}


/*
 * tok_stirng_to_literal()
 */
Token Assembler::tok_string_to_literal(const std::string& tok_string) const
{
    Token token;

    token.repr = tok_string;
    token.type = SYM_LITERAL;
    try
    {
        if(tok_string[0] == '$')
            token.val = std::stoi(tok_string.substr(1, tok_string.size()-1), nullptr, 16);
        else if(tok_string[0] == '%')
            token.val = std::stoi(tok_string.substr(1, tok_string.size()-1), nullptr, 2);
        else
            token.val = std::stoi(tok_string, nullptr, 10);
    }
    catch(std::exception& ex)
    {
        std::cerr << "[" << __func__ << "] " << ex.what() << std::endl;
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

    token = this->token_lookup.lookup(tok_string);
    // not a pre-defined token
    if(token.type == SYM_NULL)
    {
        // Check if this is a literal
        if(tok_string[0] == '$' || tok_string[0] == '%' || std::isdigit(tok_string[0]))
        {
            token = this->tok_string_to_literal(tok_string);
        }
        else if(tok_string[0] == '(')
        {
            // This must be a literal indirect (or its invalid) because if it
            // was a register indirect we would have a valid token before we 
            // check if the type was SYM_NULL.
            unsigned int close_idx = 0;         // shuts warning up, even though we don't need this kind of size
            while(tok_string[close_idx] != ')' && close_idx < tok_string.size())
                close_idx++;
            if(close_idx >= tok_string.size())
            {
                if(this->verbose)
                {
                    std::cout << "[" << __func__ << "] no matching closing paren on line " 
                        << std::dec << this->cur_line << std::endl;
                }
                return token;
            }
            // we know that tok_string[0] is an opening paren
            std::string tok_substr = tok_string.substr(1, close_idx-1);
            token = this->tok_string_to_literal(tok_substr);
            token.type = SYM_LITERAL_IND;
        } 
        // presume this is a label
        else
        {
            token.val = 0;
            token.repr = tok_string;
            token.type = SYM_LABEL;
        }
    }

    if(this->verbose)
        std::cout << "[" << __func__ << "] token is " << token.toString() << std::endl;

    return token;
}


/*
 * parse_one_arg()
 */
void Assembler::parse_arg(int arg_idx)
{
    Token token;

    token = this->next_token();
    if(token.type == SYM_NULL)
    {
        this->line_info.error = true;
        this->line_info.errstr = "Argument " + std::to_string(arg_idx) + " must be REG, LITERAL, COND, or LABEL (got "
            + std::string(token.toString()) + ")";

        if(this->verbose)
            std::cout << this->line_info.error << std::endl;
        
        return;
    }
    if(token.type == SYM_LABEL)
    {
        this->line_info.sym_arg = arg_idx;
    }
    this->line_info.args[arg_idx] = token;
}


/*
 * pase_one_or_two_arg()
 */
void Assembler::parse_one_or_two_arg(void)
{
    unsigned int start_line = this->cur_line;

    this->parse_arg(0);
    // TODO: this is bad - how complicated would it be to have a feature where 
    // you pass like an 'expected type' and it gets that instead. Note that so far the 
    // only case for this is because we have a register c and a condition c
    if(this->line_info.args[0].type == SYM_REG)
        this->line_info.args[0] = Token(SYM_COND, COND_C,  "c"); 

    this->skip_to_next_token();

    if(this->cur_line == start_line)
        this->parse_arg(1);
}

/*
 * parse_instruction()
 */
void Assembler::parse_instruction(const Token& token)
{
    // get the corresponding opcode
    this->line_info.opcode = this->opcode_lookup.get(token.repr);
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
        case INSTR_LD: 
            this->parse_arg(0);
            this->parse_arg(1);

            // FIXME: This is a dumb hack to get the arg types right
            if(this->line_info.args[0].type == SYM_COND)
            {
                this->line_info.args[0] = Token(SYM_REG, REG_C, "c");
            }
            if(this->line_info.args[1].type == SYM_COND)
            {
                this->line_info.args[1] = Token(SYM_REG, REG_C, "c");
            }
            break;

        // TODO: jr and ret can take the cond c which will get confused with the register c
        // We may need to force the type if we find that its come back as SYM_REG
        case INSTR_JR:
            this->parse_one_or_two_arg();
            break;

        case INSTR_AND: 
        case INSTR_CP:
        case INSTR_DEC:
        case INSTR_INC:
        case INSTR_OR:
        case INSTR_SUB:
        case INSTR_XOR:
            this->parse_arg(0);
            if(this->line_info.args[0].type == SYM_COND)
            {
                this->line_info.args[0] = Token(SYM_REG, REG_C, "c");
            }

            //std::cout << "[" << __func__ << "] args[0] is " << this->line_info.args[0].toString() << std::endl;
            break;

        // instructions with no operands
        case INSTR_RET:
            this->parse_arg(0);
            break;

        default:
            this->line_info.error = true;
            this->line_info.errstr = "Invalid instruction " + std::string(token.repr);
            if(this->verbose)
            {
                std::cout << "[" << __func__ << "] (line " << this->cur_line 
                    << ") : " << this->line_info.errstr << std::endl;
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

    while(!this->exhausted() && (this->cur_char != ';' || this->cur_char != '\n')) // || this->cur_char == '\0')
    {
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
            this->line_info.expr = this->read_to_line_end();
            break;

        case DIR_EQU:       // this must be a literal
            arg_token = this->next_token();
            this->line_info.data = arg_token.val;
            if(arg_token.type != SYM_LITERAL)
            {
                this->line_info.error = true;
                this->line_info.errstr = "equ directive got invalid argument " + std::string(arg_token.repr);
                break;
            }
            break;

        case DIR_INCLUDE:
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
            {
                std::cout << "[" << __func__ << "] (line " << this->cur_line 
                    << ") : " << this->line_info.errstr << std::endl;
            }
            break;
    }

    // TODO: debug, remove 
    std::cout << "[" << __func__ << "] cur line : " << std::endl;
    std::cout << this->line_info.toString() << std::endl;
}


/*
 * resolve_labels()
 */
void Assembler::resolve_labels(void)
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
                target_addr = label_addr - cur_line.addr;
                cur_line.args[cur_line.sym_arg] = Token(SYM_LITERAL, target_addr, std::to_string(target_addr));
                this->source_info.update(idx, cur_line);
            }
        }
    }
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
            std::cout << "[" << __func__ << "] added symbol [" << sym.toString() << "] to table" << std::endl;

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
    // no idea - must be a symbol/label
    else
    {
        this->line_info.line_num = this->cur_line;
    }   
    this->line_info.addr = this->cur_addr;
    this->cur_addr = this->cur_addr + instr_get_size(this->line_info.argHash());

    if(this->verbose)
    {
        std::cout << "[" << __func__ << "] current address is " << std::hex 
            << std::setw(4) << std::setfill('0') << this->cur_addr << std::endl;
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
        infile.open(filename, std::ios::binary);
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
    // TODO ; just worry about stuff that goes in the text section, not sure what the 
    // data section layout actually is yet..
    Instr cur_instr;
    LineInfo line;
    uint32_t line_hash;

    for(unsigned int idx = 0; idx < this->source_info.getNumLines(); ++idx)
    {
        line = this->source_info.get(idx);
        line_hash = line.argHash();


        if(line.type == LineType::TextLine)
        {
            auto lookup_val = instr_lookup.find(line_hash);
            if(lookup_val != instr_lookup.end())
            {
                auto instr_size = lookup_val->second;
                cur_instr.size = instr_size.second;
                if(cur_instr.size == 1)
                    cur_instr.ins  = instr_size.first;
                else if(cur_instr.size == 2 && line.args[0].type == SYM_LITERAL)
                    cur_instr.ins = (instr_size.first << 8) | (line.args[0].val & 0xFF);
                else if(cur_instr.size == 2 && line.args[1].type == SYM_LITERAL)
                    cur_instr.ins = (instr_size.first << 8) | (line.args[1].val & 0xFF);
                // ld bc, ** | ld de, ** | ld hl, ** | ld sp, **
                else if(cur_instr.size == 3 && line.args[1].type == SYM_LITERAL)
                    cur_instr.ins = (instr_size.first << 16) | (line.args[1].val & 0xFFFF);
                else if(cur_instr.size == 3 && line.args[0].type == SYM_LITERAL_IND)
                    cur_instr.ins = (instr_size.first << 16) | (line.args[0].val & 0xFFFF);
            }
            else
            {
                if(this->verbose)
                {
                    std::cerr << "[" << __func__ << "] skipping instruction " << line.toInstrString() 
                        << " with hash " << std::hex << line_hash << std::endl;
                }
                continue;
            }
            cur_instr.adr = line.addr;      
        }
        else if(line.type == LineType::DirectiveLine)
        {
            //if(line.opcode.val == DIR_DEFW || line.opcode.val == DIR_DEFB)
            if(line.expr.size() > 0 && line.evaluated == false)
                line.eval(this->source_info);

            cur_instr.adr = line.addr;
            if(line.opcode.val == DIR_DEFW)
                cur_instr.size = 2;
            else
                cur_instr.size = 1;     // TODO: this depends on defw, defb, etc
            cur_instr.ins  = uint8_t(line.data);
        }
        else
        {
            std::cerr << "[" << __func__ << "] line " << line.line_num << " has invalid line type " << std::endl;
            std::cerr << line.toString() << std::endl;
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
    if(this->verbose)
        std::cout << "[" << __func__ << "] verbose is true" << std::endl;

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
    }

    // Resolve symbols 
    this->resolve_labels();

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

void Assembler::printSource(void) const
{
    std::cout << "[" << __func__ << "] source string : " << std::endl;
    std::cout << this->source << std::endl;
}
