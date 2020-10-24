/*
 * TEST_ASSEMBLER
 * Unit tests for assembler components 
 */

#define CATCH_CONFIG_MAIN
#include "catch/catch.hpp"

#include <iostream> 
#include <iomanip>
#include <vector>
#include <string>

#include "Assembler.hpp"
#include "Source.hpp"


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
    line.addr = TEXT_START_ADDR + 6;
    line.args[0] = Token(SYM_REG, REG_B, "b");
    line.args[1] = Token(SYM_LITERAL, 8, "8");
    info.add(line);

    // ld a, b
    line.init();
    line.line_num = 12;
    line.opcode = Token(SYM_INSTR, INSTR_LD, "ld");
    line.addr = TEXT_START_ADDR + 7;
    line.args[0] = Token(SYM_REG, REG_A, "a");
    line.args[1] = Token(SYM_REG, REG_B, "b");
    info.add(line);

    // add a, 5
    line.init();
    line.line_num = 13;
    line.opcode = Token(SYM_INSTR, INSTR_ADD, "add");
    line.addr = TEXT_START_ADDR + 8;
    line.args[0] = Token(SYM_REG, REG_A, "a");
    line.args[1] = Token(SYM_LITERAL, 5, "5");
    info.add(line);

    // ld b, a
    line.init();
    line.line_num = 14;
    line.opcode = Token(SYM_INSTR, INSTR_LD, "ld");
    line.addr = TEXT_START_ADDR + 9;
    line.args[0] = Token(SYM_REG, REG_B, "b");
    line.args[1] = Token(SYM_REG, REG_A, "a");
    info.add(line);

    // ld bc, 46
    line.init();
    line.line_num = 17;
    line.opcode = Token(SYM_INSTR, INSTR_LD, "ld");
    line.addr = TEXT_START_ADDR + 10;
    line.args[0] = Token(SYM_REG, REG_BC, "bc");
    line.args[1] = Token(SYM_LITERAL, 46, "46");
    info.add(line);

    // ld h, b
    line.init();
    line.line_num = 18;
    line.opcode = Token(SYM_INSTR, INSTR_LD, "ld");
    line.addr = TEXT_START_ADDR + 11;
    line.args[0] = Token(SYM_REG, REG_H, "h");
    line.args[1] = Token(SYM_REG, REG_B, "b");
    info.add(line);

    // ld l, c
    line.init();
    line.line_num = 19;
    line.opcode = Token(SYM_INSTR, INSTR_LD, "ld");
    line.addr = TEXT_START_ADDR + 12;
    line.args[0] = Token(SYM_REG, REG_L, "l");
    line.args[1] = Token(SYM_REG, REG_C, "c");
    info.add(line);

    // ld bc, 52
    line.init();
    line.line_num = 20;
    line.opcode = Token(SYM_INSTR, INSTR_LD, "ld");
    line.addr = TEXT_START_ADDR + 13;
    line.args[0] = Token(SYM_REG, REG_BC, "bc");
    line.args[1] = Token(SYM_LITERAL, 52, "52");
    info.add(line);

    // add hl, bc
    line.init();
    line.line_num = 21;
    line.opcode = Token(SYM_INSTR, INSTR_ADD, "add");
    line.addr = TEXT_START_ADDR + 14;
    line.args[0] = Token(SYM_REG, REG_HL, "hl");
    line.args[1] = Token(SYM_REG, REG_BC, "bc");
    info.add(line);

    // ld b, h
    line.init();
    line.line_num = 22;
    line.opcode = Token(SYM_INSTR, INSTR_LD, "ld");
    line.addr = TEXT_START_ADDR + 15;
    line.args[0] = Token(SYM_REG, REG_B, "b");
    line.args[1] = Token(SYM_REG, REG_H, "h");
    info.add(line);
    // ld c, l
    line.init();
    line.line_num = 23;
    line.opcode = Token(SYM_INSTR, INSTR_LD, "ld");
    line.addr = TEXT_START_ADDR + 16;
    line.args[0] = Token(SYM_REG, REG_C, "c");
    line.args[1] = Token(SYM_REG, REG_L, "l");
    info.add(line);


    return info;
}

TEST_CASE("test_add_sub", "[classic]")
{
    Assembler assem;
    SourceInfo exp_source;

    exp_source = get_add_sub_expected_source();
    assem.loadSource(exp_source);
    assem.assemble();

}
