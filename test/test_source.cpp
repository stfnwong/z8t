/*
 * TEST_SOURCE
 * Tests for objects in Source.cpp
 */


#define CATCH_CONFIG_MAIN
#include "catch/catch.hpp"

#include <iostream> 
#include <iomanip>
#include <vector>
#include <string>

#include "Source.hpp"

TEST_CASE("test_symbol_table", "[source]")
{
    SymbolTable sym_table;
    Symbol out_symbol;

    std::vector<Symbol> inp_syms = {
        Symbol(0xDEAD, "beef_part_1"),
        Symbol(0xBEEF, "beef_part_2"),
        Symbol(0xCEED, "z0_data")
    };

    for(unsigned int idx = 0; idx < inp_syms.size(); ++idx)
    {
        sym_table.add(inp_syms[idx]);
    }
    std::cout << sym_table.toString() << std::endl;

    REQUIRE(sym_table.size() == inp_syms.size());
    REQUIRE(0xDEAD == sym_table.getAddr("beef_part_1"));
    REQUIRE("beef_part_1" == sym_table.getName(0xDEAD));
    REQUIRE(0xBEEF == sym_table.getAddr("beef_part_2"));
    REQUIRE("beef_part_2" == sym_table.getName(0xBEEF));
    REQUIRE(0xCEED == sym_table.getAddr("z0_data"));
    REQUIRE("z0_data" == sym_table.getName(0xCEED));
}


TEST_CASE("test_textline_init", "[source]")
{
    TextLine line;

    REQUIRE(line.line_num == 0);
    REQUIRE(line.addr == 0);
    REQUIRE(line.is_label == false);
    REQUIRE(line.error == false);

    for(int i = 0; i < 2; ++i)
        REQUIRE(line.args[i] == Token());
}

TEST_CASE("test_textline_hash", "[source]")
{
    TextLine line;

    std::vector<int> second_reg_types = {REG_A, REG_B, REG_C, REG_D, REG_E};
    std::vector<int> instr_opcode = {INSTR_ADD, INSTR_LD, INSTR_DEC, INSTR_POP, INSTR_EX};

    for(unsigned int reg_type = 0; reg_type < second_reg_types.size(); ++reg_type)
    {
        for(unsigned int instr_type = 0; instr_type < second_reg_types.size(); ++instr_type)
        {
            line.opcode  = Token(SYM_INSTR, instr_opcode[instr_type], "test");
            line.args[0] = Token(SYM_INSTR, REG_A, "a");
            line.args[1] = Token(SYM_INSTR, second_reg_types[reg_type], "test");

            //std::cout << line.argHash() << std::endl;
        }
    }
}

TEST_CASE("test_directive_line_single", "[source]")
{
    std::string test_expr = "1 * 50 / 2";
    DirectiveLine line;

    // setup the expression
    line.expr = test_expr;
    REQUIRE(line.data.size() == 0);
    line.eval();
    REQUIRE(line.data.size() == 1);
    REQUIRE(line.data[0] == int(1 * 50 / 2));
}

TEST_CASE("test_directive_line_multi", "[source]")
{
    std::string test_expr = "1 * 50 / 2 , 2 * 20 / 4, 3 + 3, 1 + 2";
    DirectiveLine line;

    // setup the expression
    line.expr = test_expr;
    REQUIRE(line.data.size() == 0);
    line.eval();
    REQUIRE(line.data.size() == 4);
}

