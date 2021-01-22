/*
 * ASSEMBLER
 * Z80 assembler 
 *
 * Stefan Wong 2018
 */

#ifndef __ASSEMBLER_HPP
#define __ASSEMBLER_HPP

#include <string>
#include "Source.hpp"
#include "Program.hpp"


/*
 * Assembler 
 * Takes source text and transforms into a Program object 
 * which can be written out as a program executable.
 */
class Assembler
{
    private:
        bool    verbose;
        Program program;

    // input source text
    private:
        void        init_mem(void);    // create token buffer memory
        std::string source;
        std::string filename;

    // source intermediate representation
    private:
        SourceInfo   source_info;
        LineInfo     line_info;
        Token        cur_token;
        unsigned int cur_addr;

    // token lookup
    private:
        TokenLookup  instr_lookup;
        TokenLookup  register_lookup;
        TokenLookup  condition_lookup;

    // source movement 
    private:
        bool         exhausted(void);
        void         advance(void);
        unsigned int cur_pos;
        unsigned int cur_line;
        unsigned int cur_column;
        char         cur_char;
        char*        token_buf;
        unsigned int token_buf_size;

    // token check 
    private:
        bool        is_space(void);
        bool        is_comment(void);

    // parsing 
    private:
        Token       tok_string_to_literal(const std::string& tok_string) const;
        Token       next_token(void);
        
        void        skip_whitespace(void);
        void        skip_line(void);
        void        skip_seperators(void);
        void        skip_to_next_token(void);
        void        scan_token(void);
        bool        check_ahead(void);
        // consume tokens only of a specific type
        Token       lookup_condition(const std::string& tok_str);
        Token       lookup_register(const std::string& tok_str);
        Token       lookup_instruction(const std::string& tok_str);
        Token       parse_literal(const std::string& tok_str);
        // instruction specific parsing routines 
        void        parse_jump(void);
        void        parse_ret(void);
        void        parse_rst(void);
        void        parse_call(void);
        // io 
        void        parse_in(void);
        void        parse_out(void);
        void        parse_one_literal(void);
        void        parse_arg(int arg_idx);
        void        parse_instruction(const Token& token);
        std::string read_to_line_end(void);
        void        parse_directive(const Token& token); 
        void        parse_line(void);
        bool        resolve_labels(void);

    // assemble from intermediate repr
    private:
        void assem_instr(void);

    public:
        Assembler();
        Assembler(const Assembler& that) = delete;
        Assembler(const Assembler&& that) = delete;

        void              init(void);
        void              assemble(void);
        void              setVerbose(const bool v);
        bool              getVerbose(void) const;
        int               read(const std::string& filename);
        void              loadSource(const std::string& src);
        const SourceInfo& getSourceInfo(void) const;
        unsigned int      getCurAddr(void) const;

        // getters, setters 
        Program           getProgram(void) const;
};

#endif /*__ASSEMBLER_HPP*/
