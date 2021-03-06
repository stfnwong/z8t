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


TEST_CASE("test_symbol_table", "source")
{
    SourceInfo info;
    Symbol out_symbol;

    std::vector<Symbol> inp_syms = {
        Symbol(0xDEAD, "beef_part_1"),
        Symbol(0xBEEF, "beef_part_2"),
        Symbol(0xCEED, "z0_data")
    };

    for(unsigned int idx = 0; idx < inp_syms.size(); ++idx)
        info.addSym(inp_syms[idx]);

    std::cout << info.symTableString() << std::endl;

    REQUIRE(info.getNumSyms() == inp_syms.size());
    REQUIRE(0xDEAD == info.getSymAddr("beef_part_1"));
    REQUIRE("beef_part_1" == info.getSymName(0xDEAD));
    REQUIRE(0xBEEF == info.getSymAddr("beef_part_2"));
    REQUIRE("beef_part_2" == info.getSymName(0xBEEF));
    REQUIRE(0xCEED == info.getSymAddr("z0_data"));
    REQUIRE("z0_data" == info.getSymName(0xCEED));
}

// Not really a complete test, just checking that directives fall out 
// of the lookup correctly
TEST_CASE("test_token_lookup", "source")
{
    TokenLookup token_lut;
    Token out_token;

    out_token = token_lut.lookup("xor");
    REQUIRE(out_token.type == SYM_INSTR);
    REQUIRE(out_token.val == INSTR_XOR);
    out_token = token_lut.lookup(".defw");
    REQUIRE(out_token.type == SYM_DIRECTIVE);
    REQUIRE(out_token.val == DIR_DEFW);
    out_token = token_lut.lookup(".equ");
    REQUIRE(out_token.type == SYM_DIRECTIVE);
    REQUIRE(out_token.val == DIR_EQU);
}

TEST_CASE("test_token_lookup_instructions", "source")
{
    TokenLookup token_lut(TokenSet::Instructions);
    Token out_token;

    out_token = token_lut.lookup("adc");
    REQUIRE(out_token.type == SYM_INSTR);
    REQUIRE(out_token.val == INSTR_ADC);
    // directives are also in this set
    out_token = token_lut.lookup(".defw");
    REQUIRE(out_token.type == SYM_DIRECTIVE);
    REQUIRE(out_token.val == DIR_DEFW);
    // if we ask for something that isn't a directive or 
    // instruction we get a null token
    out_token = token_lut.lookup("hl");
    REQUIRE(out_token == Token());
}

TEST_CASE("test_token_lookup_registers", "source")
{
    TokenLookup token_lut(TokenSet::Registers);
    Token out_token;

    out_token = token_lut.lookup("a");
    REQUIRE(out_token.type == SYM_REG);
    REQUIRE(out_token.val == REG_A);
    out_token = token_lut.lookup("(hl)");
    REQUIRE(out_token.type == SYM_REG);
    REQUIRE(out_token.val == REG_HL_IND);
    // anything else returns a null token
    out_token = token_lut.lookup(".defw");
    REQUIRE(out_token == Token());
    out_token = token_lut.lookup("xor");
    REQUIRE(out_token == Token());
}

TEST_CASE("test_token_lookup_conditions", "source")
{
    TokenLookup token_lut(TokenSet::Conditions);
    Token out_token;

    out_token = token_lut.lookup("a");
    REQUIRE(out_token == Token());
    // c here will always return a cond rather than a reg
    out_token = token_lut.lookup("c");
    REQUIRE(out_token.type == SYM_COND);
    REQUIRE(out_token.val == COND_C);
    out_token = token_lut.lookup("pe");
    REQUIRE(out_token.type == SYM_COND);
    REQUIRE(out_token.val == COND_PE);

    out_token = token_lut.lookup("(hl)");
    REQUIRE(out_token == Token());
    // anything else returns a null token
    out_token = token_lut.lookup(".defw");
    REQUIRE(out_token == Token());
    out_token = token_lut.lookup("xor");
    REQUIRE(out_token == Token());
}

// ======== LineInfo and related structures ======== //
TEST_CASE("test_lineinfo_init", "source")
{
    LineInfo line;

    REQUIRE(line.addr == 0);
    REQUIRE(line.line_num == 0);
    REQUIRE(line.errstr == "");
    REQUIRE(line.label == "");
    REQUIRE(line.is_label == false);
    REQUIRE(line.error == false);

    for(int i = 0; i < 2; ++i)
        REQUIRE(line.args[i] == Token());

    //REQUIRE(line.data.size() == 0);       // TODO: this will become relevant when comma-seperated fields are implemented
    REQUIRE(line.expr == "");
    REQUIRE(line.data == 0);
    REQUIRE(line.evaluated == false);
}


// ======== TEXTLINE METHODS ======== //

TEST_CASE("test_textline_hash", "source")
{
    LineInfo line;

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

TEST_CASE("test_single_part_expr", "source")
{
    SourceInfo dummy_info;
    const std::string test_expr = "1 * 50 / 2";
    LineInfo line;

    // setup the expression
    line.expr = test_expr;
    REQUIRE(line.evaluated == false);
    //REQUIRE(line.data_size() == 0);       // TODO: fix after comma-seperated expr support is implemented
    line.eval(dummy_info);
    REQUIRE(line.data_size() == 1);
    REQUIRE(line.data == int(1 * 50 / 2));
    REQUIRE(line.evaluated == true);
}

TEST_CASE("test_literal_expr", "source")
{
    SourceInfo dummy_info;
    const std::string test_expr = "16";
    LineInfo line;

    // setup the expression
    line.expr = test_expr;
    REQUIRE(line.evaluated == false);
    line.eval(dummy_info);
    REQUIRE(line.data_size() == 1);
    REQUIRE(line.data == 16);
    REQUIRE(line.evaluated == true);
}

// TODO: the implementation of multi-part exprs should be finalized after
// symbol resolution is implemented
//TEST_CASE("test_multi_part_expr", "source")
//{
//    SourceInfo dummy_info;
//    std::string test_expr = "1 * 50 / 2 , 2 * 20 / 4, 3 + 3, 1 + 2";
//    LineInfo line;
//
//    // setup the expression
//    line.expr = test_expr;
//    REQUIRE(line.data_size() == 0);
//    line.eval(dummy_info);
//
//    const std::vector<int> exp_evals = {
//        int(1 * 50 / 2),
//        int(2 * 20 / 4),
//        int(3 + 3),
//        int(1 + 2)
//    };
//
//    REQUIRE(line.data_size() == exp_evals.size());
//    for(size_t idx = 0; idx < line.data_size(); ++idx)
//        REQUIRE(line.data[idx] == exp_evals[idx]);
//}


// ======== SourceInfo tests ======== //
TEST_CASE("test_sourceinfo_init", "source")
{
    SourceInfo source;

    REQUIRE(source.getNumLines() == 0);
    LineInfo null_line = source.get(0);
    REQUIRE(null_line == LineInfo());
}

TEST_CASE("test_sourceinfo_lookup_directive_by_addr", "source")
{
    SourceInfo source;

    // create a new directive line and add to SourceInfo
    LineInfo line;
    line.type = LineType::DirectiveLine;
    line.addr = 0xBEEF;
    line.expr = "1 + 2 + 3";

    source.add(line);
    REQUIRE(source.getNumLines() == 1);

    LineInfo out_line;
    out_line = source.get(0);
    REQUIRE(out_line == line);  // obviously we can just get by index
    out_line = source.getAddr(0xBEEF);
    REQUIRE(out_line == line);
    out_line = source.getAddr(0x0);     // no such line
    REQUIRE(out_line != line);
    REQUIRE(out_line.addr == 0x0);
    REQUIRE(out_line.line_num == 0x0);
    out_line = source.getAddr(0x1);     // also no such line
    REQUIRE(out_line != line);
    REQUIRE(out_line.addr == 0x0);
    REQUIRE(out_line.line_num == 0x0);
}

TEST_CASE("test_sourceinfo_lookup_directive_by_symbol", "directive")
{
    SourceInfo source;
    const std::string sym_label = "x";
    const uint16_t sym_addr = 0xBEEF;

    // create a symbol that references the directive line
    Symbol sym(sym_addr, sym_label); 
    source.addSym(sym);

    // create a new directive line and add to SourceInfo
    LineInfo line;
    line.type = LineType::DirectiveLine;
    line.addr = sym_addr;
    line.expr = "1 + 2 + 3";

    source.add(line);
    REQUIRE(source.getNumLines() == 1);

    // To turn a symbol into a directive, we lookup the symbol address first, 
    // then seperately lookup the directive line that has that address
    LineInfo out_line;
    uint16_t out_sym_addr = source.getSymAddr(sym_label);
    REQUIRE(out_sym_addr == sym_addr);
    out_line = source.getAddr(out_sym_addr);
    REQUIRE(out_line == line);
}

//TEST_CASE("test_sourceinfo_add_get", "source")
//{
//    SourceInfo source;
//
//    LineInfo line;
//}
