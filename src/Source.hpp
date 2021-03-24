/* SOURCE
 * Intermediate representation of line info for Z80
 *
 * Stefan Wong 2018
 */

#ifndef __SOURCE_HPP
#define __SOURCE_HPP

#include <array>
#include <cstdint>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>

#include "Eval.hpp"

// TODO : find real start address
#define TEXT_START_ADDR 0x1000

/*
 * Instruction constants
 */
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
    INSTR_IN,
    INSTR_JP,
    INSTR_JR,
    INSTR_LD, 
    INSTR_INC, 
    INSTR_NOP,
    INSTR_OR,
    INSTR_OUT,
    INSTR_POP, 
    INSTR_PUSH,
    INSTR_RLA,
    INSTR_RLCA,
    INSTR_RRA,
    INSTR_RRCA,
    INSTR_RET,
    INSTR_RST,
    INSTR_SBC,
    INSTR_SCF,
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
    REG_AF,
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
    COND_PO,
    // directives
    DIR_DEFB,    // define byte
    DIR_DEFW,    // define word
    DIR_DEFS,    // define space
    DIR_DEFM,    // define message (sequence of bytes)
    DIR_DEFINE,  // define a macro (ala #define in C)
    DIR_END,     // stop assembling at this point
    DIR_EQU,     // set a labels value to the literal after equ (rather than the current address)
    DIR_INCLUDE, // include text content
    DIR_INCBIN,  // include binary content
    DIR_ORG,     // adjust current address (eg: for mapped memory)
    DIR_SEEK
} TokenType;


/*
 * Token
 */
struct Token
{
    // TODO: if we have a non-zero length repr then we need to do lookups later..
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
        bool needs_lookup(void) const;

        std::string toString(void) const;
};


// Instruction/Directive tokens 
const Token Z80_INSTRUCTIONS[] = 
{
    // Instructions 
    Token(SYM_INSTR, INSTR_ADD,  "add" ),
    Token(SYM_INSTR, INSTR_ADC,  "adc" ),
    Token(SYM_INSTR, INSTR_AND,  "and" ),
    Token(SYM_INSTR, INSTR_CALL, "call" ),
    Token(SYM_INSTR, INSTR_CCF,  "ccf" ),
    Token(SYM_INSTR, INSTR_CP,   "cp" ),
    Token(SYM_INSTR, INSTR_CPL,  "cpl" ),
    Token(SYM_INSTR, INSTR_DAA,  "daa" ),
    Token(SYM_INSTR, INSTR_DEC,  "dec" ),
    Token(SYM_INSTR, INSTR_DI,   "di" ),
    Token(SYM_INSTR, INSTR_DJNZ, "djnz" ),
    Token(SYM_INSTR, INSTR_EX,   "ex" ),
    Token(SYM_INSTR, INSTR_IN,   "in" ),
    Token(SYM_INSTR, INSTR_JP,   "jp" ),
    Token(SYM_INSTR, INSTR_JR,   "jr" ),
    Token(SYM_INSTR, INSTR_LD ,  "ld"  ),
    Token(SYM_INSTR, INSTR_INC,  "inc" ),
    Token(SYM_INSTR, INSTR_OR,   "or"),
    Token(SYM_INSTR, INSTR_OUT,  "out"),
    Token(SYM_INSTR, INSTR_POP,  "pop" ),
    Token(SYM_INSTR, INSTR_PUSH, "push" ),
    Token(SYM_INSTR, INSTR_RRA,  "rra"),
    Token(SYM_INSTR, INSTR_RRCA, "rrca"),
    Token(SYM_INSTR, INSTR_RET,  "ret"),
    Token(SYM_INSTR, INSTR_RST,  "rst"),
    Token(SYM_INSTR, INSTR_SBC,  "sbc"),
    Token(SYM_INSTR, INSTR_SUB,  "sub"),
    Token(SYM_INSTR, INSTR_XOR,  "xor"),
    // directives
    Token(SYM_DIRECTIVE, DIR_DEFB,    ".defb"),
    Token(SYM_DIRECTIVE, DIR_DEFW,    ".defw"),
    Token(SYM_DIRECTIVE, DIR_DEFS,    ".defs"),
    Token(SYM_DIRECTIVE, DIR_END,     ".end"),
    Token(SYM_DIRECTIVE, DIR_EQU,     ".equ"),
    Token(SYM_DIRECTIVE, DIR_INCLUDE, ".include"),
    Token(SYM_DIRECTIVE, DIR_ORG,     ".org"),
};

// Register tokens 
const Token Z80_REGISTERS[] = 
{
    Token(SYM_REG,  REG_A,      "a"),
    Token(SYM_REG,  REG_B,      "b"),
    Token(SYM_REG,  REG_C,      "c"),
    Token(SYM_REG,  REG_D,      "d"),
    Token(SYM_REG,  REG_E,      "e"),
    Token(SYM_REG,  REG_H,      "h"),
    Token(SYM_REG,  REG_L,      "l"),
    Token(SYM_REG,  REG_HL,     "hl"),
    Token(SYM_REG,  REG_BC,     "bc"),
    Token(SYM_REG,  REG_DE,     "de"),
    Token(SYM_REG,  REG_SP,     "sp"),
    Token(SYM_REG,  REG_BC_IND, "(bc)"),
    Token(SYM_REG,  REG_DE_IND, "(de)"),
    Token(SYM_REG,  REG_HL_IND, "(hl)"),
};

// Conditions
const Token Z80_CONDITIONS[] = 
{
    Token(SYM_COND, COND_C,  "c"),        // TODO: need to find a better way to do this..
    Token(SYM_COND, COND_NC, "nc"),
    Token(SYM_COND, COND_Z,  "z"), 
    Token(SYM_COND, COND_NZ, "nz"),
    Token(SYM_COND, COND_M,  "m"), 
    Token(SYM_COND, COND_P,  "p"), 
    Token(SYM_COND, COND_PE, "pe"),
    Token(SYM_COND, COND_PO, "po"),
};

enum struct TokenSet{ All, Instructions, Registers, Conditions }; 

/*
 * TokenLookup
 * Table of all known tokens 
 */
class TokenLookup
{
    std::unordered_map<std::string, Token> name_to_token;

    public:
        TokenLookup();
        TokenLookup(const TokenSet& tok_set);
        Token lookup(const std::string& s) const;
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
        std::string toString(void) const;
};


/*
 * LineInfo
 * Base class for line structures (either text or data)
 */
enum struct LineType {TextLine, DirectiveLine};
class SourceInfo;       // fwd delcare until we resolve how to split eval lookups

struct LineInfo
{
    // common fields 
    LineType    type;
    std::string label;
    std::string errstr;
    uint16_t    line_num;
    int16_t     addr;
    bool        is_label;
    bool        error;
    // text only fields 
    Token       opcode;             
    Token       args[2];
    int         sym_arg;
    // directive fields
    EvalResult  eval_result;  // TODO: does this become a vector of EvalResult when comma seperated args are supported?
    std::vector<std::string> expr;
    //int         data;       // TODO: this will become a vector when comma seperated fields are supported
    bool        evaluated;
    std::vector<int> data;           // generic data (eg, from a list of defb/defw)

    public:
        LineInfo();
        LineInfo(const Token& opcode, const Token& arg1, const Token& arg2);

        bool operator==(const LineInfo& that) const;
        bool operator!=(const LineInfo& that) const;
        void init(void);

        // text methods 
        uint32_t     argHash(void) const;
        // directive methods
        void         eval(const SourceInfo& info);
        unsigned int data_size(void) const;

        // stringify
        std::string  toString(void) const;
        std::string  toInstrString(void) const;
        std::string  diff(const LineInfo& that);
};

/*
 * SourceInfo
 * Wraps a collection of LineInfo objects that represents each
 * of the lines in the source file.
 * NOTE that when include is supported (ie: compilation of multiple
 * files) that the implementation here may need to slightly change
 */
class SourceInfo
{
    private: 
        std::vector<LineInfo> info;
        // maps address to index in info vector
        std::unordered_map <int16_t, unsigned int> directive_addr_lut;
        // TODO : do we also want to have a lookup for 
        // symbol table 
        std::vector<Symbol> syms;
        Symbol null_symbol;

    public:
        SourceInfo();
        //SourceInfo(const SourceInfo& that) = default;

        void         init(void);
        void         add(const LineInfo& l);
        bool         hasError(void) const;
        LineInfo     get(const unsigned int idx) const;
        LineInfo     getAddr(const int16_t addr) const;
        void         update(const unsigned int idx, const LineInfo& l);
        unsigned int getNumLines(void) const;
        void         toFile(const std::string& filename) const;

        // symbol table functions
        void         addSym(const Symbol& s);
        void         updateSym(const unsigned int idx, const Symbol& s);
        Symbol       getSym(const unsigned int idx) const;
        uint16_t     getSymAddr(const std::string& label) const;
        std::string  getSymName(const uint16_t addr) const;
        unsigned int getNumSyms(void) const;
        std::string  symTableString(void) const;

        std::string  toString(void) const;
};

#endif /*__SOURCE_HPP*/
