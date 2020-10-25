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
#include <unordered_map>

// TODO : find real start address
#define TEXT_START_ADDR 0x0

/*
 * Instruction constants
 */
// TODO : these should just be one giant enum so that the hashing works correctly
typedef enum {
    INSTR_ADD, 
    INSTR_ADC,
    INSTR_AND, 
    INSTR_CALL,
    INSTR_CCF,
    INSTR_CP,
    INSTR_CPL,
    INSTR_DAA,
    INSTR_DEC, 
    INSTR_DI,
    INSTR_DJNZ,
    INSTR_EX,
    INSTR_JP,
    INSTR_JR,
    INSTR_LD, 
    INSTR_INC, 
    INSTR_OR,
    INSTR_OUT,
    INSTR_POP, 
    INSTR_PUSH,
    INSTR_RRA,
    INSTR_RRCA,
    INSTR_RET,
    INSTR_RST,
    INSTR_SBC,
    INSTR_SUB,
    INSTR_XOR
} Z80_INSTR;


/*
 * TokenType
 */
typedef enum {
    SYM_NULL,
    SYM_EOF,
    SYM_DIRECTIVE,
    SYM_INSTR,
    SYM_LITERAL,
    SYM_LITERAL_IND,
    SYM_LABEL,
    SYM_REG,
    SYM_COND,
    // registers 
    REG_NULL,
    REG_A, 
    REG_B, 
    REG_C, 
    REG_D, 
    REG_E, 
    REG_H, 
    REG_L, 
    REG_IXH, 
    REG_IXL, 
    REG_IYH, 
    REG_IYL, 
    REG_HL, 
    REG_BC,
    REG_DE,
    REG_SP,
    REG_BC_IND,     // indirect, ie: (BC)
    REG_HL_IND,     // indirect, ie: (HL)
    REG_DE_IND,     // indirect, ie: (DE)
    // conditions
    COND_C,
    COND_NC, 
    COND_Z,
    COND_NZ,
    COND_M,
    COND_P,
    COND_PE,
    COND_PO
} TokenType;


/*
 * Token
 */
struct Token
{
    TokenType   type;
    int         val;
    std::string repr;

    public:
        Token();
        Token(const TokenType& type, int val, const std::string& repr);
        Token(const Token& that);

        bool operator==(const Token& that) const;
        bool operator!=(const Token& that) const;
        void init(void);

        std::string toString(void) const;
};

// hash on a key like 
//
// SYM_TYPE | ARG1_VAL | ARG2_VAL
// ie:

// Collection of all valid tokens 
const Token Z80_TOKENS[] =
{
    // Instructions 
    Token(SYM_INSTR, INSTR_ADD, "add" ),
    Token(SYM_INSTR, INSTR_ADC, "adc" ),
    Token(SYM_INSTR, INSTR_AND, "and" ),
    Token(SYM_INSTR, INSTR_CALL, "call" ),
    Token(SYM_INSTR, INSTR_CCF, "ccf" ),
    Token(SYM_INSTR, INSTR_CP, "cp" ),
    Token(SYM_INSTR, INSTR_CPL, "cpl" ),
    Token(SYM_INSTR, INSTR_DAA, "daa" ),
    Token(SYM_INSTR, INSTR_DEC, "dec" ),
    Token(SYM_INSTR, INSTR_DI, "di" ),
    Token(SYM_INSTR, INSTR_DJNZ, "djnz" ),
    Token(SYM_INSTR, INSTR_EX, "ex" ),
    Token(SYM_INSTR, INSTR_JP, "jp" ),
    Token(SYM_INSTR, INSTR_JR, "jr" ),
    Token(SYM_INSTR, INSTR_LD , "ld"  ),
    Token(SYM_INSTR, INSTR_INC, "inc" ),
    Token(SYM_INSTR, INSTR_OR, "or"),
    Token(SYM_INSTR, INSTR_OUT, "out"),
    Token(SYM_INSTR, INSTR_POP, "pop" ),
    Token(SYM_INSTR, INSTR_PUSH, "push" ),
    Token(SYM_INSTR, INSTR_RRA, "rra"),
    Token(SYM_INSTR, INSTR_RRCA, "rrca"),
    Token(SYM_INSTR, INSTR_RET, "ret"),
    Token(SYM_INSTR, INSTR_SBC, "sbc"),
    Token(SYM_INSTR, INSTR_SUB, "sub"),
    Token(SYM_INSTR, INSTR_XOR, "xor"),
    // Registers 
    Token(SYM_REG,  REG_A,  "a"),
    Token(SYM_REG,  REG_B,  "b"),
    Token(SYM_REG,  REG_C,  "c"),
    Token(SYM_REG,  REG_D,  "d"),
    Token(SYM_REG,  REG_E,  "e"),
    Token(SYM_REG,  REG_H,  "h"),
    Token(SYM_REG,  REG_L,  "l"),
    Token(SYM_REG,  REG_HL, "hl"),
    Token(SYM_REG,  REG_BC, "bc"),
    Token(SYM_REG,  REG_DE, "de"),
    Token(SYM_REG,  REG_SP, "sp"),
    Token(SYM_REG,  REG_BC_IND, "(bc)"),
    Token(SYM_REG,  REG_DE_IND, "(de)"),
    Token(SYM_REG,  REG_HL_IND, "(hl)"),
    // directives
    // conditions
    Token(SYM_COND, COND_C,  "C"), 
    Token(SYM_COND, COND_NC, "NC"),
    Token(SYM_COND, COND_Z,  "Z"), 
    Token(SYM_COND, COND_NZ, "NZ"),
    Token(SYM_COND, COND_M,  "M"), 
    Token(SYM_COND, COND_P,  "P"), 
    Token(SYM_COND, COND_PE, "PE"),
    Token(SYM_COND, COND_PO, "PO"),
};


/*
 * TokenLookup
 * Table of all known tokens 
 */
class TokenLookup
{
    std::unordered_map<std::string, Token> name_to_token;

    public:
        TokenLookup();
        Token lookup(const std::string& s) const;
};


/*
 * List of Accepted instruction Opcodes
 * TODO: consider replacing these with just a token since the real 
 * opcode depends on the combination of args
 */
const Token Z80_OPCODES[] = 
{
    Token(SYM_INSTR, INSTR_ADD, "add" ),
    Token(SYM_INSTR, INSTR_AND, "and" ),
    Token(SYM_INSTR, INSTR_DEC, "dec" ),
    Token(SYM_INSTR, INSTR_LD , "ld"  ),
    Token(SYM_INSTR, INSTR_INC, "inc" ),
    Token(SYM_INSTR, INSTR_POP, "pop" ),
    Token(SYM_INSTR, INSTR_PUSH, "push"),
};


class OpcodeLookup
{
    std::unordered_map<int, Token> val_to_opcode;
    std::unordered_map<std::string, Token> name_to_opcode;

    public:
        OpcodeLookup();
        Token get(const int val) const;
        Token get(const std::string& name) const;
}; 


/* 
 * Symbol
 */
struct Symbol
{
    uint16_t    addr;
    std::string label;

    public:
        Symbol();
        Symbol(const uint16_t addr, const std::string& label);

        void init(void);
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
        SymbolTable(const SymbolTable& that) = default;

    public:
        SymbolTable();

        void         add(const Symbol& s);
        void         update(const unsigned int idx, const Symbol& s);
        Symbol       get(const unsigned int idx) const;
        uint16_t     getAddr(const std::string& label) const;
        std::string  getName(const uint16_t addr) const;
        void         init(void);
        unsigned int size(void) const;

        std::string  toString(void) const;
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
    Token       opcode;     
    Token       args[2];
    int8_t      sym_arg;        // which arg has a symbol (so we don't have to check later)
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
        uint32_t argHash(void) const;

        std::string toString(void) const;
        std::string diff(const TextLine& that);
        std::string toInstrString(void) const;
};


// TODO ; in keeping with the text/data segment distinction, this should be at some point
// renamed into something that indicates that its a collection of TextLine objects
// TODO : are iterators acceptable enough that I could implement one here?
class SourceInfo
{
    private: 
        std::vector<TextLine> info;

    public:
        SourceInfo();
        SourceInfo(const SourceInfo& that) = default;

        void init(void);
        void add(const TextLine& l);
        TextLine get(const unsigned int idx) const;
        void update(const unsigned int idx, const TextLine& l);
        unsigned int getNumLines(void) const;
        void toFile(const std::string& filename) const;
};

#endif /*__SOURCE_HPP*/
