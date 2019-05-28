/* SOURCE
 * Intermediate representation of line info for Z80
 *
 * Stefan Wong 2018
 */

#ifndef __SOURCE_HPP
#define __SOURCE_HPP

#include <cstdint>
#include <sstream>
#include <string>
#include <vector>

/*
 * TokenType
 */
typedef enum {
    SYM_EOF,
    SYM_INSTR,
    SYM_LITERAL,
    SYM_LABEL,
    SYM_REG
} TokenType;

const std::vector <std::string> token_type_str = {
    "EOF",
    "INSTR",
    "LITERAL",
    "LABEL",
    "REG"
};

/*
 * Token
 */
typedef struct 
{
    std::string val;
    TokenType   type;
} Token;

/* 
 * Opcode 
 * Holds the mnemonic string and opcode value of 
 * an assembly opcode 
 */
typedef struct 
{
    public:
        uint16_t    code;
        std::string mnemonic;
} Opcode;

class InstrTable
{
    private:
        std::vector<Opcode> instrs;
        
    public:
        InstrTable();
        ~InstrTable();
        InstrTable(const InstrTable& that) = delete;    // TODO; copy ctor later
        void init(void);
        void add(const Opcode& o);
        void get(const std::string& mnemonic, Opcode& o) const;
        void get(const uint16_t opcode, Opcode& o) const;
        std::string getMnemonic(const uint16_t opcode) const;
        void getIdx(const unsigned int idx, Opcode& o) const;

        // generic getters 
        unsigned int getNumOps(void) const;
        std::string getStr(const unsigned int idx) const;
};

/* 
 * Symbol
 */
typedef struct {
    uint16_t    addr;
    std::string label;
} Symbol;

/*
 * SymbolTable 
 */
class SymbolTable
{
    private:
        std::vector<Symbol> syms;

    public:
        SymbolTable();
        ~SymbolTable();
        SymbolTable(const SymbolTable& that);

        void         add(const Symbol& s);
        void         update(const unsigned int idx, const Symbol& s);
        Symbol       get(const unsigned int idx) const;
        uint16_t     getAddr(const std::string& label) const;
        void         init(void);
        unsigned int getNumSyms(void) const;
        // debug 
        void         dump(void);
};


/*
 * LineInfo
 */
class LineInfo
{
    public:
        // avoid having a large number of setters and getters 
        std::string symbol;
        std::string label;
        std::string errstr;
        Opcode      opcode;
        Token       arg1;
        Token       arg2;
        uint16_t    line_num;
        uint16_t    addr;
        bool        is_label;
        bool        error;

    public:
        LineInfo();
        ~LineInfo();
        LineInfo(const LineInfo& that);

        void init(void);
        std::string toString(void);
};


class SourceInfo
{
    private: 
        std::vector<LineInfo> info;

    public:
        SourceInfo();
        ~SourceInfo();
        SourceInfo(const SourceInfo& that);

        void add(const LineInfo& l);
        LineInfo get(const unsigned int idx) const;
        unsigned int getNumLines(void) const;
        void toFile(const std::string& filename) const;
};


#endif /*__SOURCE_HPP*/
