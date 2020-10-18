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
 * Instructions
 */
typedef enum {
    INSTR_ADD, INSTR_AND, INSTR_DEC, INSTR_LD, INSTR_INC, INSTR_POP, INSTR_PUSH
} Z80_INSTR;


/*
 * TokenType
 */
typedef enum {
    SYM_NULL,
    SYM_EOF,
    SYM_INSTR,
    SYM_LITERAL,
    SYM_LABEL,
    SYM_REG,
    SYM_COND
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
struct Token
{
    std::string val;
    TokenType   type;

    public:
        Token();
        Token(const std::string& val, const TokenType& type);
        Token(const Token& that);

        bool operator==(const Token& that) const;
        bool operator!=(const Token& that) const;

        std::string toString(void) const;
};

/*
 * Argument
 */
struct Argument
{
    TokenType type;
    int val;
    std::string repr;

    public:
        Argument();
        Argument(const TokenType& type, int val);
        Argument(const TokenType& type, int val, const std::string& s);
        Argument(const Argument& that);

        bool operator==(const Argument& that) const;
        bool operator!=(const Argument& that) const;
        void init(void);
        std::string toString(void) const;
};

/* 
 * Opcode 
 * Holds the mnemonic string and opcode value of 
 * an assembly opcode 
 */
struct Opcode
{
    int        code;
    std::string mnemonic;

    public:
        Opcode();
        Opcode(int opcode, const std::string& mnemonic);

        bool operator==(const Opcode& that) const;
        bool operator!=(const Opcode& that) const;
        void init(void);

        std::string toString(void) const;
};


// TODO : genericize these tables 
class InstrTable
{
    private:
        std::vector<Opcode> instrs;
        Opcode null_opcode;
        
    public:
        InstrTable();
        InstrTable(const InstrTable& that) = delete;    // TODO; copy ctor later

        void init(void);
        void add(const Opcode& o);
        // TODO : return copy of T 
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
struct Symbol{

    uint16_t    addr;
    std::string label;

    public:
        Symbol();
        Symbol(const uint16_t addr, const std::string& label);

        bool operator==(const Symbol& that) const;
        bool operator!=(const Symbol& that) const;
};

/*
 * SymbolTable 
 */
class SymbolTable
{
    private:
        std::vector<Symbol> syms;
        Symbol null_symbol;

    private:
        SymbolTable(const SymbolTable& that) = delete;

    public:
        SymbolTable();

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
 * TextLine
 * Class representing a line from the source file that will go in the text segment.
 */
struct TextLine
{
    // avoid having a large number of setters and getters 
    std::string symbol;
    std::string label;
    std::string errstr;
    Opcode      opcode;
    Argument    args[2];
    uint16_t    line_num;
    uint16_t    addr;
    bool        is_label;
    bool        error;

    public:
        TextLine();
        TextLine(const TextLine& that);

        bool operator==(const TextLine& that) const;
        bool operator!=(const TextLine& that) const;
        void init(void);
        std::string diff(const TextLine& that);
        std::string toString(void);
};


// TODO ; in keeping with the text/data segment distinction, this should be at some point
// renamed into something that indicates that its a collection of TextLine objects
class SourceInfo
{
    private: 
        std::vector<TextLine> info;

    public:
        SourceInfo();
        //SourceInfo(const SourceInfo& that);

        void add(const TextLine& l);
        TextLine get(const unsigned int idx) const;
        unsigned int getNumLines(void) const;
        void toFile(const std::string& filename) const;
};


#endif /*__SOURCE_HPP*/
