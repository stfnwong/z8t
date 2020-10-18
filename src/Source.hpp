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


/*
 * Instruction constants
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
    SYM_DIRECTIVE,
    SYM_INSTR,
    SYM_LITERAL,
    SYM_LITERAL_IND,
    SYM_LABEL,
    SYM_REG,
    SYM_COND
} TokenType;

// Register names 
typedef enum Z80_REG
{
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
    REG_BC_IND,     // indirect, ie: (BC)
    REG_HL_IND,     // indirect, ie: (HL)
    REG_DE_IND      // indirect, ie: (DE)
} Z80_REG;


// Condition codes
typedef enum
{
    COND_C,
    COND_NC, 
    COND_Z,
    COND_NZ,
    COND_M,
    COND_P,
    COND_PE,
} Z80_COND;


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

// Collection of all valid tokens 
const Token Z80_TOKENS[] =
{
    // Instructions 
    Token(SYM_INSTR, INSTR_ADD, "add" ),
    Token(SYM_INSTR, INSTR_AND, "and" ),
    Token(SYM_INSTR, INSTR_DEC, "dec" ),
    Token(SYM_INSTR, INSTR_LD , "ld"  ),
    Token(SYM_INSTR, INSTR_INC, "inc" ),
    Token(SYM_INSTR, INSTR_POP, "pop" ),
    Token(SYM_INSTR, INSTR_PUSH, "push"),
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
    // directives
    // conditions
    Token(SYM_COND, COND_C,  "C"), 
    Token(SYM_COND, COND_NC, "NC"),
    Token(SYM_COND, COND_Z,  "Z"), 
    Token(SYM_COND, COND_NZ, "NZ"),
    Token(SYM_COND, COND_M,  "M"), 
    Token(SYM_COND, COND_P,  "P"), 
    Token(SYM_COND, COND_PE, "PE"),
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
 * Opcode 
 * Holds the mnemonic string and opcode value of 
 * an assembly opcode 
 */
struct Opcode
{
    int         code;
    std::string mnemonic;

    public:
        Opcode();
        Opcode(int opcode, const std::string& mnemonic);

        bool operator==(const Opcode& that) const;
        bool operator!=(const Opcode& that) const;
        void init(void);

        std::string toString(void) const;
};


const Opcode Z80_OPCODES[] = 
{
    Opcode(INSTR_ADD, "add" ),
    Opcode(INSTR_AND, "and" ),
    Opcode(INSTR_DEC, "dec" ),
    Opcode(INSTR_LD , "ld"  ),
    Opcode(INSTR_INC, "inc" ),
    Opcode(INSTR_POP, "pop" ),
    Opcode(INSTR_PUSH, "push"),
};


class OpcodeLookup
{
    std::unordered_map<int, Opcode> val_to_opcode;
    std::unordered_map<std::string, Opcode> name_to_opcode;

    public:
        OpcodeLookup();
        Opcode get(const int val) const;
        Opcode get(const std::string& name) const;
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
    Opcode      opcode;     // TODO : make this just another Token?
    Token       args[2];
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
