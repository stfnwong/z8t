/*
 * LEXER 
 * Lexer for Z80 assembly.
 *
 * Stefan Wong 2018
 */

#ifndef __LEXER_HPP
#define __LEXER_HPP

#include <string>
#include <cstdint>
#include "Source.hpp"

// Register names 
typedef enum {
    REG_A, REG_B, REG_C, REG_D, REG_E, REG_H, REG_L, REG_IXH, REG_IXL, REG_IYH, REG_IYL, REG_HL, REG_BC
} Z80_REG;

// Instructions 
typedef enum {
    INSTR_ADD, INSTR_AND, INSTR_DEC, INSTR_LD, INSTR_INC, INSTR_POP, INSTR_PUSH
} Z80_INSTR;

/*
 * List of valid instruction codes for SYM_INSTR tokens
 */
static Opcode z80_instr_codes[] = {
    {INSTR_ADD,  "add"},
    {INSTR_AND,  "and"},
    {INSTR_DEC,  "dec"},
    {INSTR_LD,   "ld"},
    {INSTR_INC,  "inc"},
    {INSTR_POP,  "pop"},
    {INSTR_PUSH, "push"}
};

static Opcode z80_reg_names[] = {
    {REG_A, "a"},
    {REG_B, "b"},
    {REG_C, "c"},
    {REG_D, "d"},
    {REG_E, "e"},
    {REG_H, "h"},
    {REG_L, "l"},
    {REG_HL, "hl"},
    {REG_BC, "bc"}
};

/*
 * Lexer 
 */
class Lexer
{
    private:
        bool verbose;
        void init_mem(void);

    private:
        SourceInfo   source_info;
        TextLine     line_info;
        Token        cur_token;
        unsigned int cur_addr;

    private:
        // Symbol/token info 
        InstrTable instr_table;
        InstrTable reg_table;
        SymbolTable symbol_table;
        void init_instr_table(void);
        void init_reg_table(void);

    private:
        // source movement 
        bool exhausted(void);
        void advance(void);
        unsigned int cur_pos;
        unsigned int cur_line;
        char cur_char;
        char* token_buf;
        unsigned int token_buf_size;

    // token extraction
    //private:
    //    Token extractLiteral(void);
    //    Token extractRegister(void);

    private:
        // token check 
        bool is_space(void);
        bool is_comment(void);

    private:
        // parsing 
        void skip_whitespace(void);
        void skip_comment(void);
        void skip_seperators(void);
        void scan_token(void);
        void next_token(void);
        void parse_two_arg(void);


        void parse_line(void);

    private:
        std::string source;
        std::string filename;

    // prevent copy and move construction
    private:
        Lexer(const Lexer& that) = delete;
        Lexer(const Lexer&& that) = delete;

    public:
        Lexer();
        ~Lexer();

        void lex(void);
        int read(const std::string& filename);
        SourceInfo getSource(void) const;
        
        void setVerbose(const bool v);
        bool getVerbose(void) const;
};

#endif /*__LEXER_HPP*/
