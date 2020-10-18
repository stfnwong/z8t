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
#include "Register.hpp"
#include "Source.hpp"

// TODO : find real start address
#define TEXT_START_ADDR 0x0

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
        RegisterTable reg_table;
        RegisterMap reg_map;
        CondMap cond_map;
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

    private:
        // token check 
        bool is_space(void);
        bool is_comment(void);

    // token extraction
    private:
        Argument parse_arg(const Token& token) const;
        Argument extract_literal(const std::string& token_str);
        Argument extract_register(const std::string& token_str);

    private:
        // parsing 
        void skip_whitespace(void);
        void skip_comment(void);
        void skip_seperators(void);
        void scan_token(void);
        void next_token(void);

        void parse_one_arg(void);
        void parse_two_arg(void);

        void parse_instruction(void);
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
        void loadSource(const std::string& src);
        const SourceInfo& getSource(void) const;
        
        void setVerbose(const bool v);
        bool getVerbose(void) const;
};

#endif /*__LEXER_HPP*/
