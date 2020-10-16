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
#include "Lexer.hpp"

Lexer::Lexer() 
{
    this->verbose        = false;
    this->cur_pos        = 0;
    this->cur_line       = 0;
    this->cur_addr       = 0;     // TODO : find start address
    this->token_buf_size = 256;
    this->init_mem();
    this->init_instr_table();
    this->init_reg_table();
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

/*
 * advance()
 */
void Lexer::advance(void)
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
    while(this->cur_char == ' ' || 
          this->cur_char == '\n' || 
          this->cur_char == '\t')
        this->advance();
}


/*
 * extract_literal()
 */
Argument Lexer::extract_literal(void)
{
    Argument arg;
    unsigned int idx = 0;
    bool base16 = false;

    while(idx < (this->token_buf_size-1))
    {
        if(this->cur_char == '$')
        {
            base16 = true;
            this->advance();
            continue;
        }
        if(!std::isdigit(this->cur_char))
            break;
        this->token_buf[idx] = this->cur_char;
        idx++;
        this->advance();
    }
    this->token_buf[idx] = '\0';
    
    if(idx == 0)
        return arg;

    if(base16)
        arg.val = std::stoi(this->token_buf, nullptr, 16);
    else
        arg.val = std::stoi(this->token_buf, nullptr, 10);
    arg.type = SYM_LITERAL;
    arg.repr = std::string(this->token_buf);

    // TODO : debug, remove 
    std::cout << "[" << __func__ << "] arg : " << arg.toString() << std::endl;

    return arg;
}

/*
 * extract_register()
 */
Argument Lexer::extract_register(void)
{
    Argument arg;

    // TODO: need to deal with parentheses
    this->next_token();
    std::cout << "[" << __func__ << "] cur_token :" << 
        this->cur_token.toString() << std::endl;

    if(this->cur_token.type != SYM_REG)
    {
        this->line_info.errstr = "Expected register (got " + this->cur_token.toString() + ")";
        this->line_info.error = true;
        return arg;
    }

    arg.type = SYM_REG;
    arg.val  = this->reg_map.getIdx(std::string(this->cur_token.val));
    arg.repr = std::string(this->cur_token.val);

    return arg;
}

/* 
 * skip_comment()
 */
void Lexer::skip_comment(void)
{
    while(this->cur_char != '\n')
        this->advance();

    this->advance();    // skip over the newline char
}

/*
 * scan_token()
 */
void Lexer::scan_token(void)
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
            << this->cur_line << ") : token_buf contains <" 
            << std::string(this->token_buf) << "> " << std::endl;
    }
}

/*
 * next_token()
 */
void Lexer::next_token(void)
{
    Opcode op;
    std::string token_str; 

    this->scan_token();     // load token into token buffer
    token_str = std::string(this->token_buf);

    // TODO: return op here, if invalid then it should have a null type
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
    else if(token_str[0] == '$' || std::isdigit(token_str[0]))
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
        std::cout << "[" << __func__ << "] got token "
            << this->cur_token.toString() << std::endl;
    }
}

/*
 * parse_two_arg()
 */
void Lexer::parse_two_arg(void)
{
    // First arg must be register 
    Argument arg;

    arg = this->extract_register();
    if(arg.type != SYM_REG)
    {
        this->line_info.error = true;
        this->line_info.errstr = "First argument must be register (got " + arg.toString() + ")";
        return;
    }   
    this->line_info.args[0] = arg;

    // try to extract a literal.
    arg = this->extract_literal();
    if(arg.type == SYM_LITERAL)
    {
        this->line_info.args[1] = arg;
        return ;
    }

    arg = this->extract_register();
    if(arg.type != SYM_REG)
    {
        this->line_info.error = true;
        this->line_info.errstr = "Second argument must be register or literal (got " + arg.toString() + ")";
        return;
    }   

    this->line_info.args[1] = arg;
}

/*
 * parse_instruction()
 */
void Lexer::parse_instruction(void)
{
    Opcode op;

    if(this->verbose)
    {
        std::cout << "[" << __func__ << "] processing INSTR symbol" << std::endl;
    }

    this->instr_table.get(this->cur_token.val, op);
    this->line_info.opcode = op;
    switch(op.code)
    {
        case INSTR_ADD:
            this->parse_two_arg();
            break;

        case INSTR_AND: 
            this->parse_two_arg();
            break;

        case INSTR_LD: 
            this->parse_two_arg();
            break;

        default:
            this->line_info.error = true;
            this->line_info.errstr = "Invalid instruction ";
            if(this->verbose)
            {
                std::cout << "[" << __func__ << "] (line " << this->cur_line 
                    << ") : " << this->line_info.errstr << std::endl;
            }
    }
}

/*
 * parse_line()
 */
void Lexer::parse_line(void)
{
    Symbol s;

    if(this->verbose)
        std::cout << "[" << __func__ << "] parsing line " << this->cur_line << std::endl;

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

    // Check if we have an instruction
    if(this->cur_token.type == SYM_INSTR)
    {
        this->parse_instruction();
    }
    else
    {
        this->line_info.error = true;
        this->line_info.errstr = "No valid instructions";
        if(this->verbose)
        {
            std::cout << "[" << __func__ << "] (line " << this->cur_line 
                << ") : " << this->line_info.errstr << std::endl;
        }
    }   

    this->line_info.addr = this->cur_addr;
    this->cur_addr++;
}

/*
 * lex()
 */
void Lexer::lex(void)
{
    this->cur_line = 1;
    this->cur_pos = 0;

    while(!this->exhausted())
    {
        if(this->is_space())
        {
            std::cout << "[" << __func__ << "] skipping whitespace on line "
                << std::dec << this->cur_line << std::endl;
            this->skip_whitespace();
            continue;
        }
        if(this->is_comment())
        {
            //std::cout << "[" << __func__ << "] skipping comment on line "
            //    << std::dec << this->cur_line << std::endl;
            this->skip_comment();
            continue;
        }
        this->parse_line();
        this->source_info.add(this->line_info);
        //std::cout << "[" << __func__ << "] current line " << std::endl;
        //std::cout << this->line_info.toString() << std::endl;
    }

    if(this->verbose)
    {
        std::cout << "[" << __func__ << "] parsed " 
            << this->source_info.getNumLines() << " lines of source"
            << std::endl;
    }


    // Resolve symbols 
}

/*
 * read()
 */
int Lexer::read(const std::string& filename)
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
void Lexer::loadSource(const std::string& src)
{
    this->source = src;
}

/*
 * getSource()
 */
const SourceInfo& Lexer::getSource(void) const
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
