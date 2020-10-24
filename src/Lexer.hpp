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
        TokenLookup token_lookup;
        OpcodeLookup opcode_lookup;
        SymbolTable symbol_table;

    private:
        // source movement 
        bool exhausted(void);
        void advance(void);
        unsigned int cur_pos;
        unsigned int cur_line;
        char         cur_char;
        char*        token_buf;
        unsigned int token_buf_size;

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

        Token tok_string_to_literal(const std::string& tok_string) const;
        Token next_token(void);

        void parse_one_arg(void);
        void parse_two_arg(void);

        void parse_instruction(const Token& token);
        void parse_line(void);

        void resolve_labels(void);

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
