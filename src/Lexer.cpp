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
    this->cur_addr       = TEXT_START_ADDR;     
    this->token_buf_size = 128;
    this->init_mem();
}

Lexer::~Lexer()
{
    delete[] this->token_buf;
}

/*
 * init_mem()
 */
void Lexer::init_mem(void)
{
    this->token_buf = new char [this->token_buf_size];
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
    if(this->cur_char == ' ' || this->cur_char == '\n')
        return true;

    return false;
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
 * tok_stirng_to_literal()
 */
Token Lexer::tok_string_to_literal(const std::string& tok_string) const
{
    Token token;

    try
    {
        if(tok_string[0] == '$')
        {
            token.repr = tok_string;
            token.val = std::stoi(tok_string.substr(1, tok_string.size()-1), nullptr, 16);
            token.type = SYM_LITERAL;
        }
        else
        {
            token.repr = tok_string;
            token.val = std::stoi(tok_string, nullptr, 10);
            token.type = SYM_LITERAL;
        }
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
Token Lexer::next_token(void) 
{
    Token token;
    std::string tok_string;
    
    this->scan_token();
    tok_string = std::string(this->token_buf);  

    token = this->token_lookup.lookup(tok_string);
    // not a pre-defined token
    if(token.type == SYM_NULL)
    {
        // TODO: could become jump table
        // Check if this is a literal
        if(tok_string[0] == '$' || std::isdigit(tok_string[0]))
        {
            token = this->tok_string_to_literal(tok_string);
        }
        else if(tok_string[0] == '(')
        {
            // TODO : find closing paren, get literal, set type
            //int idx = 0;
            //while(tok_string[idx] != ')')
            //    idx++;
            // TODO : hack version - replace with something more robust 
            
            std::cout << "[" << __func__ << "] when indirection is implemented, this branch will parse indirections" << std::endl; 
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

    return token;
}


/*
 * parse_one_arg()
 */
void Lexer::parse_one_arg(void)
{
    Token token;

    token = this->next_token();
    if(token.type == SYM_NULL)
    {
        this->line_info.error = true;
        this->line_info.errstr = "First argument must be REG, LITERAL, COND, or LABEL (got "
            + std::string(token.toString()) + ")";

        if(this->verbose)
            std::cout << this->line_info.error << std::endl;
        
        return;
    }
    if(token.type == SYM_LABEL)
    {
        this->line_info.is_label = true;
        this->line_info.label = token.val;
    }
    this->line_info.args[0] = token;
}


/*
 * parse_two_arg()
 */
void Lexer::parse_two_arg(void)
{
    Token token;

    token = this->next_token();
    if(token.type == SYM_NULL)
    {
        this->line_info.error = true;
        this->line_info.errstr = "First argument must be REG, LITERAL, COND, or LABEL (got "
            + std::string(token.toString()) + ")";

        if(this->verbose)
            std::cout << this->line_info.error << std::endl;
        
        return;
    }
    if(token.type == SYM_LABEL)
    {
        this->line_info.is_label = true;
        this->line_info.label = token.val;
    }
    this->line_info.args[0] = token;

    token = this->next_token();
    if(token.type == SYM_NULL)
    {
        this->line_info.error = true;
        this->line_info.errstr = "Second argument must be REG, LITERAL, COND, or LABEL (got "
            + std::string(token.toString()) + ")";

        if(this->verbose)
            std::cout << this->line_info.error << std::endl;
        
        return;
    }
    this->line_info.args[1] = token;
}

/*
 * parse_instruction()
 */
void Lexer::parse_instruction(const Token& token)
{
    // get the corresponding opcode
    this->line_info.opcode = this->opcode_lookup.get(token.repr);

    // Since this is already a token object we can just
    // jump based on the value
    switch(token.val)
    {
        case INSTR_ADD:
        case INSTR_LD: 
            this->parse_two_arg();
            break;

        case INSTR_AND: 
        case INSTR_DEC:
        case INSTR_OR:
            this->parse_one_arg();
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
    Token token;

    if(this->verbose)
        std::cout << "[" << __func__ << "] parsing line " << this->cur_line << std::endl;

    this->line_info.init();
    this->line_info.line_num = this->cur_line;
    token = this->next_token();

    if(token.type == SYM_LABEL)
    {
        // add symbol to table
        s.label = token.val;
        s.addr  = this->cur_addr;
        this->symbol_table.add(s);
        this->line_info.label = s.label;

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
        std::cout << "[" << __func__ << "] would handle directive here" << std::endl;
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