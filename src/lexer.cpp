/*
 * LEXER 
 * Lexer for Z80 assembly.
 *
 * Stefan Wong 2018
 */

#include <iostream>
#include <iomanip>
#include <cctype>
#include <fstream>
#include "lexer.hpp"

Lexer::Lexer() 
{
    this->verbose = false;
    this->cur_pos = 0;
    this->cur_line = 0;
    this->cur_addr = 0;     // TODO : find start address
    this->token_buf_size = 256;
    this->init_mem();
    this->init_instr_table();
}

Lexer::~Lexer()
{
    delete[] this->token_buf;
}

void Lexer::init_mem(void)
{
    this->token_buf = new char [this->token_buf_size];
}

/*
 * init_instr_table()
 */
void Lexer::init_instr_table(void)
{
    for(const Opcode& code : z80_instr_codes)
        this->instr_table.add(code);
}

/*
 * init_reg_table()
 */
void Lexer::init_reg_table(void)
{
    for(const Opcode& code : z80_reg_names)
        this->reg_table.add(code);
}

/*
 * exhausted()
 */
bool Lexer::exhausted(void)
{
    return (this->cur_char == '\0') ? true : false;
}

void Lexer::advance(void)
{
    this->cur_pos++;
    this->cur_char = this->source[this->cur_pos];    
    if(this->cur_char == '\n')
        this->cur_line++;
}

/* 
 * is_space()
 */
bool Lexer::is_space(void)
{
    return (this->cur_char == ' ') ? true : false;
}

/* 
 * is_comment()
 */
bool Lexer::is_comment(void)
{
    return (this->cur_char == ';') ? true : false;
}

/* 
 * skip_whitespace()
 */
void Lexer::skip_whitespace(void)
{
    while(this->cur_char == ' ')
        this->advance();
}

/* 
 * skip_comment()
 */
void Lexer::skip_comment(void)
{
    while(this->cur_char != '\n')
        this->advance();
}

/*
 * scan_token()
 */
void Lexer::scan_token(void)
{
    unsigned int idx = 0;
    this->skip_whitespace();
    //this->skip_seperators();
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
        this->token_buf[idx] = this->cur_char;
        this->advance();
        idx++;
    }

    this->token_buf[idx] = '\0';
    if(this->cur_char == ',')   // skip over any seperators
        this->advance();

    if(this->verbose)
    {
        std::cout << "[" << __FUNCTION__ << "] (line " << std::dec 
            << this->cur_line << ") : token_buf contains <" 
            << std::string(this->token_buf) << "> " << std::endl;
    }
}

// Line parsing 
void Lexer::next_token(void)
{
    Opcode op;
    std::string token_str; 

    this->scan_token();     // load token into token buffer
    token_str = std::string(this->token_buf);
    this->instr_table.get(token_str, op);

    // Instructions 
    if(op.mnemonic != "\0")
    {
        this->cur_token.type = SYM_INSTR;
        this->cur_token.val  = token_str;
        goto TOKEN_END;
    }
    // named register
    this->reg_table.get(token_str, op);
    if(op.mnemonic != "\0")
    {
        this->cur_token.type = SYM_REG;
        this->cur_token.val  = token_str;
        goto TOKEN_END;
    }
    // hex literal or numeric literal 
    else if(token_str[0] == '$' || isdigit(token_str[0]))
    {
        this->cur_token.type = SYM_LITERAL;
        this->cur_token.val  = token_str;
        goto TOKEN_END;
    }
    else
    {
        this->cur_token.type = SYM_LABEL;
        this->cur_token.val  = token_str;
    }

TOKEN_END:
    if(this->verbose)
    {
        std::cout << "[" << __FUNCTION__ << "] got token <" 
            << token_type_str[(unsigned int) this->cur_token.type] 
            << "> : "  << this->cur_token.val << std::endl;
    }
}

/*
 * parse_line()
 */
void Lexer::parse_line(void)
{
    Symbol s;
    Opcode op;

    if(this->verbose)
        std::cout << "[" << __FUNCTION__ << "] parsing line " << this->cur_line << std::endl;

    this->line_info.init();
    this->line_info.line_num = this->cur_line;
    this->next_token();
    // Check if we have a label 
    if(this->cur_token.type == SYM_LABEL)
    {
        // TODO : remove trailing ':'
        s.label = this->cur_token.val;
        s.addr  = this->cur_addr;
        this->symbol_table.add(s);
        // Scan the next token
        this->next_token();
        this->line_info.line_num = this->cur_line;
    }

    if(this->cur_token.type == SYM_INSTR)
    {
        this->instr_table.get(this->cur_token.val, op);
        switch(op.code)
        {
            case INSTR_ADD:
                std::cout << "[" << __FUNCTION__ << "] got ADD" << std::endl;
                break;

            case INSTR_AND: 
                std::cout << "[" << __FUNCTION__ << "] got AND" << std::endl;
                break;

            case INSTR_LD: 
                std::cout << "[" << __FUNCTION__ << "] got LD" << std::endl;
                break;

            default:
                this->line_info.error = true;
                this->line_info.errstr = "Invalid instruction ";
                if(this->verbose)
                {
                    std::cout << "[" << __FUNCTION__ << "] (line " << this->cur_line 
                        << ") : " << this->line_info.errstr << std::endl;
                }
                goto LINE_END;
        }
    }
    else
    {
        this->line_info.error = true;
        this->line_info.errstr = "No valid instructions";
        if(this->verbose)
        {
            std::cout << "[" << __FUNCTION__ << "] (line " << this->cur_line 
                << ") : " << this->line_info.errstr << std::endl;
        }
    }   

LINE_END:
    this->line_info.addr = this->cur_addr;
    this->cur_addr++;
}

/*
 * lex()
 */
void Lexer::lex(void)
{
    this->cur_line = 1;

    while(!this->exhausted())
    {
        if(this->is_space())
        {
            this->skip_whitespace();
            continue;
        }
        if(this->is_comment())
        {
            this->skip_comment();
            continue;
        }
        this->parse_line();
        this->source_info.add(this->line_info);
    }

    // Resolve symbols 
}


int Lexer::read(const std::string& filename)
{
    std::ifstream infile;
    std::string line;

    try {
        infile.open(filename, std::ios::binary);
    }
    catch(std::ios_base::failure& e){
        std::cerr << "[" << __FUNCTION__ << "] caught exception " 
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
 * getSource()
 */
SourceInfo Lexer::getSource(void) const
{
    return this->source_info;
}

void Lexer::setVerbose(const bool v)
{
    this->verbose = v;
}
bool Lexer::getVerbose(void) const
{
    return this->verbose;
}
