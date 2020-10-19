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

TEST_CASE("test_symbol_table", "[classic]")
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
