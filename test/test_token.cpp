/*
 * TEST_TOKEN
 * Tests for both the Token object and Lookup tables.
 */

#define CATCH_CONFIG_MAIN
#include "catch/catch.hpp"

#include <iostream> 
#include <iomanip>
#include <vector>
#include <string>

#include "Source.hpp"


// TODO : just test all reserved symbols
//

const Token instr_expected_tokens[] = 
{
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


const Token register_expected_tokens[] = {
    Token(SYM_REG,  REG_A,  "a"),
    Token(SYM_REG,  REG_B,  "b"),
    //Token(SYM_REG,  REG_C,  "c"),     // TODO : need typed lookup to solve this...
    Token(SYM_REG,  REG_D,  "d"),
    Token(SYM_REG,  REG_E,  "e"),
    Token(SYM_REG,  REG_H,  "h"),
    Token(SYM_REG,  REG_L,  "l"),
    Token(SYM_REG,  REG_HL, "hl"),
    Token(SYM_REG,  REG_BC, "bc"),
};

const Token condition_expected_tokens[] = {
    Token(SYM_COND, COND_C,  "c"),        // TODO: need to find a better way to do this..
    Token(SYM_COND, COND_NC, "nc"),
    Token(SYM_COND, COND_Z,  "z"), 
    Token(SYM_COND, COND_NZ, "nz"),
    Token(SYM_COND, COND_M,  "m"), 
    Token(SYM_COND, COND_P,  "p"), 
    Token(SYM_COND, COND_PE, "pe"),
    Token(SYM_COND, COND_PO, "po"),
};


TEST_CASE("test_instruction_lookup", "token")
{
    TokenLookup instr_lut(TokenSet::Instructions);
    Token out_token;


    for(const Token& exp_token : instr_expected_tokens)
    {
        out_token = instr_lut.lookup(exp_token.repr);
        if(out_token != exp_token)
        {
            std::cout << "Token mismatch. Looked for [" << exp_token.repr << "]" << std::endl;
            std::cout << "Expected : " << exp_token.toString() << std::endl;
            std::cout << "Got      : " << out_token.toString() << std::endl;
        }
        REQUIRE(out_token == exp_token);
    }

    // all other tokens will return nothing useful
    for(const Token& cond_token : condition_expected_tokens)
    {
        out_token = instr_lut.lookup(cond_token.repr);
        REQUIRE(out_token == Token());
    }
}

TEST_CASE("test_register_lookup", "token")
{
    TokenLookup register_lut(TokenSet::Registers);
    Token out_token;

    for(const Token& exp_token : register_expected_tokens)
    {
        out_token = register_lut.lookup(exp_token.repr);
        if(out_token != exp_token)
        {
            std::cout << "Token mismatch. Looked for [" << exp_token.repr << "]" << std::endl;
            std::cout << "Expected : " << exp_token.toString() << std::endl;
            std::cout << "Got      : " << out_token.toString() << std::endl;
        }
        REQUIRE(out_token == exp_token);
    }
}

TEST_CASE("test_condition_lookup", "token")
{
    TokenLookup condition_lut(TokenSet::Conditions);
    Token out_token;

    for(const Token& exp_token : condition_expected_tokens)
    {
        out_token = condition_lut.lookup(exp_token.repr);
        if(out_token != exp_token)
        {
            std::cout << "Token mismatch. Looked for [" << exp_token.repr << "]" << std::endl;
            std::cout << "Expected : " << exp_token.toString() << std::endl;
            std::cout << "Got      : " << out_token.toString() << std::endl;
        }
        REQUIRE(out_token == exp_token);
    }
}
