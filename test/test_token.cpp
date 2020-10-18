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

TEST_CASE("test_token_lookup", "[classic]")
{
    TokenLookup lut;
    Token exp_token;
    Token out_token;

    // what if we get instructions?
    exp_token.type = SYM_INSTR;
    exp_token.val = INSTR_ADD;
    exp_token.repr = "add";

    out_token = lut.lookup("add");
    REQUIRE(out_token == exp_token);

    exp_token.type = SYM_INSTR;
    exp_token.val = INSTR_DEC;
    exp_token.repr = "dec";

    out_token = lut.lookup("dec");
    REQUIRE(out_token == exp_token);

    // what if we get registers?
    exp_token.type = SYM_REG;
    exp_token.val = REG_HL;
    exp_token.repr = "hl";

    out_token = lut.lookup("hl");
    REQUIRE(out_token == exp_token);
    // what if its a one letter register ? 
    exp_token.type = SYM_REG;
    exp_token.val = REG_C;
    exp_token.repr = "c";

    out_token = lut.lookup("c");
    REQUIRE(out_token == exp_token);

    // if its a capital c then it should be a condition
    exp_token.type = SYM_COND;
    exp_token.val = COND_C;
    exp_token.repr = "C";

    out_token = lut.lookup("C");
    REQUIRE(out_token == exp_token);

}
