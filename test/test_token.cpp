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

const Token expected_tokens[] = 
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
    Token(SYM_INSTR, INSTR_SBC,  "sbc"),
    Token(SYM_INSTR, INSTR_SUB,  "sub"),
    Token(SYM_INSTR, INSTR_XOR,  "xor"),

    Token(SYM_REG,  REG_A,  "a"),
    Token(SYM_REG,  REG_B,  "b"),
    //Token(SYM_REG,  REG_C,  "c"),     // TODO : need typed lookup to solve this...
    Token(SYM_REG,  REG_D,  "d"),
    Token(SYM_REG,  REG_E,  "e"),
    Token(SYM_REG,  REG_H,  "h"),
    Token(SYM_REG,  REG_L,  "l"),
    Token(SYM_REG,  REG_HL, "hl"),
    Token(SYM_REG,  REG_BC, "bc"),

    //Token(SYM_COND, COND_C,  "C"),        // TODO: this could be solved by a different lexer that knows what 'types' to expect
    Token(SYM_COND, COND_NC, "nc"),
    Token(SYM_COND, COND_Z,  "z"), 
    Token(SYM_COND, COND_NZ, "nz"),
    Token(SYM_COND, COND_M,  "m"), 
    Token(SYM_COND, COND_P,  "p"), 
    Token(SYM_COND, COND_PE, "pe"),
};


TEST_CASE("test_token_lookup", "[classic]")
{
    TokenLookup lut;
    Token out_token;

    for(const Token& exp_token : expected_tokens)
    {
        out_token = lut.lookup(exp_token.repr);
        if(out_token != exp_token)
        {
            std::cout << "Token mismatch. Looked for [" << exp_token.repr << "]" << std::endl;
            std::cout << "Expected : " << exp_token.toString() << std::endl;
            std::cout << "Got      : " << out_token.toString() << std::endl;
        }
        REQUIRE(out_token == exp_token);
    }
}
