/* TEST_LEXER 
 * Tests for Z80 psuedo-assembler 
 *
 * Stefan Wong 2018
 */

#define CATCH_CONFIG_MAIN
#include "catch/catch.hpp"

#include <iostream> 
#include <iomanip>
#include <vector>
#include <string>

#include "Lexer.hpp"
#include "Source.hpp"

constexpr const bool GLOBAL_VERBOSE = true;
const std::string add_sub_filename = "asm/add_sub.asm";
const std::string indirect_filename = "asm/indirect_test.asm";


SourceInfo lex_helper(const std::string& filename)
{
    int status;
    SourceInfo info;
    Lexer lexer;

    lexer.setVerbose(GLOBAL_VERBOSE);
    std::cout << "\t Reading file " << filename << std::endl;
    status = lexer.read(filename);
    if(status != 0)
    {
        std::cout << "[" << __func__ << "] failed to read file " << filename << std::endl;
        return info;
    }

    std::cout << "\t Lexing file " << filename << std::endl;
    lexer.lex();
    info = lexer.getSource();

    return info;
}



// TODO ; need to update addresses once assembler and lexer are merged
SourceInfo get_add_sub_expected_source(void)
{
    SourceInfo info;
    TextLine line;

    // ld a, 8
    line.init();
    line.line_num = 5;
    line.opcode = Token(SYM_INSTR, INSTR_LD, "ld");
    line.addr = TEXT_START_ADDR;    
    line.args[0] = Token(SYM_REG, REG_A, "a");
    line.args[1] = Token(SYM_LITERAL, 8, "8");
    info.add(line);

    // add a, 10
    line.init();
    line.line_num = 6;
    line.opcode = Token(SYM_INSTR, INSTR_ADD, "add");
    line.addr = TEXT_START_ADDR + 1;    
    line.args[0] = Token(SYM_REG, REG_A, "a");
    line.args[1] = Token(SYM_LITERAL, 10, "10");
    info.add(line);
    
    // ld hl, $FC00
    line.init();
    line.line_num = 7;
    line.opcode = Token(SYM_INSTR, INSTR_LD, "ld");
    line.addr = TEXT_START_ADDR + 2;    
    line.args[0] = Token(SYM_REG, REG_HL, "hl");
    line.args[1] = Token(SYM_LITERAL, 0xFC00, "$fc00");
    info.add(line);

    // ld bc, $00BB
    line.init();
    line.line_num = 8;
    line.opcode = Token(SYM_INSTR, INSTR_LD, "ld");
    line.addr = TEXT_START_ADDR + 3;    
    line.args[0] = Token(SYM_REG, REG_BC, "bc");
    line.args[1] = Token(SYM_LITERAL, 0x00BB, "$00bb");
    info.add(line);

    // ld b, 8
    line.init();
    line.line_num = 11;
    line.opcode = Token(SYM_INSTR, INSTR_LD, "ld");
    line.addr = TEXT_START_ADDR + 4;    
    line.args[0] = Token(SYM_REG, REG_B, "b");
    line.args[1] = Token(SYM_LITERAL, 8, "8");
    info.add(line);

    // ld a, b
    line.init();
    line.line_num = 12;
    line.opcode = Token(SYM_INSTR, INSTR_LD, "ld");
    line.addr = TEXT_START_ADDR + 5;    
    line.args[0] = Token(SYM_REG, REG_A, "a");
    line.args[1] = Token(SYM_REG, REG_B, "b");
    info.add(line);

    // add a, 5
    line.init();
    line.line_num = 13;
    line.opcode = Token(SYM_INSTR, INSTR_ADD, "add");
    line.addr = TEXT_START_ADDR + 6;    
    line.args[0] = Token(SYM_REG, REG_A, "a");
    line.args[1] = Token(SYM_LITERAL, 5, "5");
    info.add(line);

    // ld b, a
    line.init();
    line.line_num = 14;
    line.opcode = Token(SYM_INSTR, INSTR_LD, "ld");
    line.addr = TEXT_START_ADDR + 7;    
    line.args[0] = Token(SYM_REG, REG_B, "b");
    line.args[1] = Token(SYM_REG, REG_A, "a");
    info.add(line);

    // ld bc, 46
    line.init();
    line.line_num = 17;
    line.opcode = Token(SYM_INSTR, INSTR_LD, "ld");
    line.addr = TEXT_START_ADDR + 8;    
    line.args[0] = Token(SYM_REG, REG_BC, "bc");
    line.args[1] = Token(SYM_LITERAL, 46, "46");
    info.add(line);

    // ld h, b
    line.init();
    line.line_num = 18;
    line.opcode = Token(SYM_INSTR, INSTR_LD, "ld");
    line.addr = TEXT_START_ADDR + 9;    
    line.args[0] = Token(SYM_REG, REG_H, "h");
    line.args[1] = Token(SYM_REG, REG_B, "b");
    info.add(line);

    // ld l, c
    line.init();
    line.line_num = 19;
    line.opcode = Token(SYM_INSTR, INSTR_LD, "ld");
    line.addr = TEXT_START_ADDR + 10;    
    line.args[0] = Token(SYM_REG, REG_L, "l");
    line.args[1] = Token(SYM_REG, REG_C, "c");
    info.add(line);

    // ld bc, 52
    line.init();
    line.line_num = 20;
    line.opcode = Token(SYM_INSTR, INSTR_LD, "ld");
    line.addr = TEXT_START_ADDR + 11;    
    line.args[0] = Token(SYM_REG, REG_BC, "bc");
    line.args[1] = Token(SYM_LITERAL, 52, "52");
    info.add(line);

    // add hl, bc
    line.init();
    line.line_num = 21;
    line.opcode = Token(SYM_INSTR, INSTR_ADD, "add");
    line.addr = TEXT_START_ADDR + 12;    
    line.args[0] = Token(SYM_REG, REG_HL, "hl");
    line.args[1] = Token(SYM_REG, REG_BC, "bc");
    info.add(line);

    // ld b, h
    line.init();
    line.line_num = 22;
    line.opcode = Token(SYM_INSTR, INSTR_LD, "ld");
    line.addr = TEXT_START_ADDR + 13;    
    line.args[0] = Token(SYM_REG, REG_B, "b");
    line.args[1] = Token(SYM_REG, REG_H, "h");
    info.add(line);
    // ld c, l
    line.init();
    line.line_num = 23;
    line.opcode = Token(SYM_INSTR, INSTR_LD, "ld");
    line.addr = TEXT_START_ADDR + 14;    
    line.args[0] = Token(SYM_REG, REG_C, "c");
    line.args[1] = Token(SYM_REG, REG_L, "l");
    info.add(line);

    return info;
}


TEST_CASE("test_add_sub", "[classic]")
{
    SourceInfo lex_source;
    SourceInfo exp_source;

    lex_source = lex_helper(add_sub_filename);
    std::cout << "\t Lexer generated " << lex_source.getNumLines() << " line of output" << std::endl;

    // Check intermediate results
    exp_source = get_add_sub_expected_source();
    REQUIRE(lex_source.getNumLines() == exp_source.getNumLines());
    for(unsigned int i = 0; i < exp_source.getNumLines(); ++i)
    {
        TextLine exp_line = exp_source.get(i);
        TextLine lex_line = lex_source.get(i);

        if(exp_line != lex_line)
        {
            std::cout << "Error in line " << i+1 << "/" 
                << exp_source.getNumLines() << std::endl;

            std::cout << exp_line.diff(lex_line) << std::endl;

            std::cout << "Expected :" << std::endl;
            std::cout << exp_line.toString() << std::endl;

            std::cout << "Got :" << std::endl;
            std::cout << lex_line.toString() << std::endl;
        }

        REQUIRE(exp_line == lex_line);
    }
}

/*
 * Indirect expected output
 */
SourceInfo get_indirect_expected_source(void)
{
    SourceInfo info;
    TextLine line;

    // ld (bc),a 
    line.init();
    line.line_num = 3;
    line.opcode = Token(SYM_INSTR, INSTR_LD, "ld");
    line.addr = TEXT_START_ADDR;
    line.args[0] = Token(SYM_REG, REG_BC_IND, "(bc)");
    line.args[1] = Token(SYM_REG, REG_A, "a");
    info.add(line);
    // ld (de), a
    line.init();
    line.line_num = 4;
    line.opcode = Token(SYM_INSTR, INSTR_LD, "ld");
    line.addr = TEXT_START_ADDR + 1;
    line.args[0] = Token(SYM_REG, REG_DE_IND, "(de)");
    line.args[1] = Token(SYM_REG, REG_A, "a");
    info.add(line);
    // add a, (hl)
    line.init();
    line.line_num = 5;
    line.opcode = Token(SYM_INSTR, INSTR_ADD, "add");
    line.addr = TEXT_START_ADDR + 2;
    line.args[0] = Token(SYM_REG, REG_A, "a");
    line.args[1] = Token(SYM_REG, REG_HL_IND, "(hl)");
    info.add(line);
    // ld (hl), e 
    line.init();
    line.line_num = 6;
    line.opcode = Token(SYM_INSTR, INSTR_LD, "ld");
    line.addr = TEXT_START_ADDR + 3;
    line.args[0] = Token(SYM_REG, REG_HL_IND, "(hl)");
    line.args[1] = Token(SYM_REG, REG_E, "e");
    info.add(line);
    // ld a, (de)
    line.init();
    line.line_num = 8;
    line.opcode = Token(SYM_INSTR, INSTR_LD, "ld");
    line.addr = TEXT_START_ADDR + 4;
    line.args[0] = Token(SYM_REG, REG_A, "a");
    line.args[1] = Token(SYM_REG, REG_DE_IND, "(de)");
    info.add(line);
    // ld a, (bc)
    line.init();
    line.line_num = 9;
    line.opcode = Token(SYM_INSTR, INSTR_LD, "ld");
    line.addr = TEXT_START_ADDR + 5;
    line.args[0] = Token(SYM_REG, REG_A, "a");
    line.args[1] = Token(SYM_REG, REG_BC_IND, "(bc)");
    info.add(line);
    // ld e, (hl)
    line.init();
    line.line_num = 10;
    line.opcode = Token(SYM_INSTR, INSTR_LD, "ld");
    line.addr = TEXT_START_ADDR + 6;
    line.args[0] = Token(SYM_REG, REG_E, "e");
    line.args[1] = Token(SYM_REG, REG_HL_IND, "(hl)");
    info.add(line);
    // ld hl, ($20)
    line.init();
    line.line_num = 12;
    line.opcode = Token(SYM_INSTR, INSTR_LD, "ld");
    line.addr = TEXT_START_ADDR + 7;
    line.args[0] = Token(SYM_REG, REG_HL, "hl");
    line.args[1] = Token(SYM_LITERAL_IND, 32, "$20");
    info.add(line);

    return info;
}


TEST_CASE("test_indirect", "[classic]")
{
    SourceInfo lex_source;
    SourceInfo exp_source;

    lex_source = lex_helper(indirect_filename);
    std::cout << "\t Lexer generated " << lex_source.getNumLines() << " line of output" << std::endl;

    // Check intermediate results
    exp_source = get_indirect_expected_source();
    for(unsigned int i = 0; i < exp_source.getNumLines(); ++i)
    {
        TextLine exp_line = exp_source.get(i);
        TextLine lex_line = lex_source.get(i);

        if(exp_line != lex_line)
        {
            std::cout << "Error in line " << i+1 << "/" 
                << exp_source.getNumLines() << std::endl;

            std::cout << exp_line.diff(lex_line) << std::endl;

            std::cout << "Expected :" << std::endl;
            std::cout << exp_line.toString() << std::endl;

            std::cout << "Got :" << std::endl;
            std::cout << lex_line.toString() << std::endl;
        }

        REQUIRE(exp_line == lex_line);
    }
}
