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
#include "Program.hpp"

constexpr const bool GLOBAL_VERBOSE = false;
const std::string add_sub_filename = "asm/add_sub.asm";
const std::string indirect_filename = "asm/indirect_test.asm";
const std::string gcd_filename = "asm/gcd.asm";
const std::string expr_filename = "asm/expr.asm";
const std::string label_resolve_filename = "asm/label_resolve.asm";
const std::string ret_lookahead_filename = "asm/ret_lookahead.asm";

// TODO: a test which goes through the entire lookup table and checks
// that all instructions return valid pairs
//TEST_CASE("", "")
//{
//
//}

TEST_CASE("test_assembler_init", "assembler")
{
    Assembler assem;

    REQUIRE(assem.getCurAddr() == TEXT_START_ADDR);
}

/*
 * helper function for lexing source
 */
SourceInfo lex_helper(const std::string& filename)
{
    int status;
    SourceInfo info;
    Assembler assem;

    assem.setVerbose(GLOBAL_VERBOSE);
    std::cout << "\t Reading file " << filename << std::endl;
    status = assem.read(filename);
    if(status != 0)
    {
        std::cout << "[" << __func__ << "] failed to read file " << filename << std::endl;
        return info;
    }

    std::cout << "\t Lexing file " << filename << std::endl;
    assem.assemble();
    info = assem.getSourceInfo();

    return info;
}

void source_check_helper(const SourceInfo& exp_source, const SourceInfo& lex_source)
{
    if(GLOBAL_VERBOSE)
    {
        for(unsigned int i = 0; i < lex_source.getNumLines(); ++i)
        {
            LineInfo cur_line = lex_source.get(i);
            std::cout << cur_line.toString() << std::endl;
        }
    }

    // Check intermediate results
    for(unsigned int i = 0; i < exp_source.getNumLines(); ++i)
    {
        LineInfo exp_line = exp_source.get(i);
        LineInfo lex_line = lex_source.get(i);

        // note that eval happens during instruction assemble, so we need to do it here manually
        if(!lex_line.evaluated)
            lex_line.eval(lex_source);

        if(exp_line != lex_line)
        {
            std::cout << "Error in line " << std::dec << i+1 << "/" 
                << std::dec << exp_source.getNumLines() << std::endl;

            std::cout << exp_line.diff(lex_line) << std::endl;

            std::cout << "Expected :" << std::endl;
            std::cout << exp_line.toString() << std::endl;

            std::cout << "Got :" << std::endl;
            std::cout << lex_line.toString() << std::endl;
        }
        REQUIRE(exp_line == lex_line);
    }
}

void program_check_helper(const Program& exp_program, const Program& out_program)
{
    if(GLOBAL_VERBOSE)
    {
        std::cout << "Expected program :" << std::endl;
        for(unsigned int idx = 0; idx < exp_program.length(); ++idx)
            std::cout << "[" << std::setw(4) << std::dec << idx << "] " << exp_program.get(idx).toString() << std::endl;

        std::cout << "Output program :" << std::endl;
        for(unsigned int idx = 0; idx < out_program.length(); ++idx)
            std::cout << "[" << std::setw(4) << std::dec << idx << "] " << out_program.get(idx).toString() << std::endl;
    }

    // Check instructions 
    for(unsigned int idx = 0; idx < exp_program.length(); ++idx)
    {
        Instr out_instr = out_program.get(idx);
        Instr exp_instr = exp_program.get(idx);

        if(out_instr != exp_instr)
        {
            std::cout << "Difference in instruction " << idx << std::endl;
            std::cout << "Expected : " << exp_instr.toString() << std::endl;
            std::cout << "Got      : " << out_instr.toString() << std::endl;
        }
        REQUIRE(out_instr == exp_instr);
    }
}


// ======== LEXER TESTS ======== //

SourceInfo get_add_sub_expected_source(void)
{
    SourceInfo info;
    LineInfo line;

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
    line.addr = TEXT_START_ADDR + 2;
    line.args[0] = Token(SYM_REG, REG_A, "a");
    line.args[1] = Token(SYM_LITERAL, 10, "10");
    info.add(line);
    // ld hl, $FC00
    line.init();
    line.line_num = 7;
    line.opcode = Token(SYM_INSTR, INSTR_LD, "ld");
    line.addr = TEXT_START_ADDR + 4;
    line.args[0] = Token(SYM_REG, REG_HL, "hl");
    line.args[1] = Token(SYM_LITERAL, 0xFC00, "$fc00");
    info.add(line);
    // ld bc, $00BB
    line.init();
    line.line_num = 8;
    line.opcode = Token(SYM_INSTR, INSTR_LD, "ld");
    line.addr = TEXT_START_ADDR + 7;
    line.args[0] = Token(SYM_REG, REG_BC, "bc");
    line.args[1] = Token(SYM_LITERAL, 0x00BB, "$00bb");
    info.add(line);
    // ld b, 8
    line.init();
    line.line_num = 11;
    line.opcode = Token(SYM_INSTR, INSTR_LD, "ld");
    line.addr = TEXT_START_ADDR + 10;
    line.args[0] = Token(SYM_REG, REG_B, "b");
    line.args[1] = Token(SYM_LITERAL, 8, "8");
    info.add(line);
    // ld a, b
    line.init();
    line.line_num = 12;
    line.opcode = Token(SYM_INSTR, INSTR_LD, "ld");
    line.addr = TEXT_START_ADDR + 12;
    line.args[0] = Token(SYM_REG, REG_A, "a");
    line.args[1] = Token(SYM_REG, REG_B, "b");
    info.add(line);
    // add a, 5
    line.init();
    line.line_num = 13;
    line.opcode = Token(SYM_INSTR, INSTR_ADD, "add");
    line.addr = TEXT_START_ADDR + 13;
    line.args[0] = Token(SYM_REG, REG_A, "a");
    line.args[1] = Token(SYM_LITERAL, 5, "5");
    info.add(line);
    // ld b, a
    line.init();
    line.line_num = 14;
    line.opcode = Token(SYM_INSTR, INSTR_LD, "ld");
    line.addr = TEXT_START_ADDR + 15;
    line.args[0] = Token(SYM_REG, REG_B, "b");
    line.args[1] = Token(SYM_REG, REG_A, "a");
    info.add(line);
    // ld bc, 46
    line.init();
    line.line_num = 17;
    line.opcode = Token(SYM_INSTR, INSTR_LD, "ld");
    line.addr = TEXT_START_ADDR + 16;
    line.args[0] = Token(SYM_REG, REG_BC, "bc");
    line.args[1] = Token(SYM_LITERAL, 46, "46");
    info.add(line);
    // ld h, b
    line.init();
    line.line_num = 18;
    line.opcode = Token(SYM_INSTR, INSTR_LD, "ld");
    line.addr = TEXT_START_ADDR + 19;
    line.args[0] = Token(SYM_REG, REG_H, "h");
    line.args[1] = Token(SYM_REG, REG_B, "b");
    info.add(line);
    // ld l, c
    line.init();
    line.line_num = 19;
    line.opcode = Token(SYM_INSTR, INSTR_LD, "ld");
    line.addr = TEXT_START_ADDR + 20;
    line.args[0] = Token(SYM_REG, REG_L, "l");
    line.args[1] = Token(SYM_REG, REG_C, "c");
    info.add(line);
    // ld bc, 52
    line.init();
    line.line_num = 20;
    line.opcode = Token(SYM_INSTR, INSTR_LD, "ld");
    line.addr = TEXT_START_ADDR + 21;
    line.args[0] = Token(SYM_REG, REG_BC, "bc");
    line.args[1] = Token(SYM_LITERAL, 52, "52");
    info.add(line);
    // add hl, bc
    line.init();
    line.line_num = 21;
    line.opcode = Token(SYM_INSTR, INSTR_ADD, "add");
    line.addr = TEXT_START_ADDR + 24;
    line.args[0] = Token(SYM_REG, REG_HL, "hl");
    line.args[1] = Token(SYM_REG, REG_BC, "bc");
    info.add(line);
    // ld b, h
    line.init();
    line.line_num = 22;
    line.opcode = Token(SYM_INSTR, INSTR_LD, "ld");
    line.addr = TEXT_START_ADDR + 25;
    line.args[0] = Token(SYM_REG, REG_B, "b");
    line.args[1] = Token(SYM_REG, REG_H, "h");
    info.add(line);
    // ld c, l
    line.init();
    line.line_num = 23;
    line.opcode = Token(SYM_INSTR, INSTR_LD, "ld");
    line.addr = TEXT_START_ADDR + 26;
    line.args[0] = Token(SYM_REG, REG_C, "c");
    line.args[1] = Token(SYM_REG, REG_L, "l");
    info.add(line);

    return info;
}


TEST_CASE("test_lex_add_sub", "lexer")
{
    SourceInfo lex_source;
    SourceInfo exp_source;

    lex_source = lex_helper(add_sub_filename);
    std::cout << "\t Lexer generated " << std::dec << lex_source.getNumLines() << " line of output" << std::endl;

    // Check intermediate results
    exp_source = get_add_sub_expected_source();
    REQUIRE(lex_source.getNumLines() == exp_source.getNumLines());

    source_check_helper(exp_source, lex_source);
}

/*
 * Indirect expected output
 */
SourceInfo get_indirect_expected_source(void)
{
    SourceInfo info;
    LineInfo line;

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


TEST_CASE("test_lex_indirect", "lexer")
{
    SourceInfo lex_source;
    SourceInfo exp_source;

    lex_source = lex_helper(indirect_filename);
    std::cout << "\t Lexer generated " << std::dec << lex_source.getNumLines() << " line of output" << std::endl;

    // Check intermediate results
    exp_source = get_indirect_expected_source();

    source_check_helper(exp_source, lex_source);
}

/*
 * GCD source
 */
SourceInfo get_gcd_expected_source(void)
{
    SourceInfo info;
    LineInfo line;

    // gcd: cp, b 
    line.init();
    line.line_num = 9;  // label should be attached to following line
    line.opcode = Token(SYM_INSTR, INSTR_CP, "cp");
    line.addr = TEXT_START_ADDR;
    line.args[0] = Token(SYM_REG, REG_B, "b");
    line.label = "gcd";
    line.is_label = true;
    info.add(line);
    // ret z 
    line.init();
    line.line_num = 10;  
    line.opcode = Token(SYM_INSTR, INSTR_RET, "ret");
    line.addr = TEXT_START_ADDR + 1;
    line.args[0] = Token(SYM_COND, COND_Z, "z");
    info.add(line);
    // jr c, else
    line.init();
    line.line_num = 11;  
    line.opcode = Token(SYM_INSTR, INSTR_JR, "jr");
    line.addr = TEXT_START_ADDR + 2;
    line.args[0] = Token(SYM_COND, COND_C, "c");
    line.args[1] = Token(SYM_LITERAL, 3, "3");  // add to PC, even though distance is 5 bytes we only need to jump 3 more (since this instr is already 2 bytes long)
    line.sym_arg = 1;
    info.add(line);
    // sub b
    line.init();
    line.line_num = 12;  
    line.opcode = Token(SYM_INSTR, INSTR_SUB, "sub");
    line.addr = TEXT_START_ADDR + 4;
    line.args[0] = Token(SYM_REG, REG_B, "b");
    info.add(line);
    // jr gcd
    line.init();
    line.line_num = 13;  
    line.opcode = Token(SYM_INSTR, INSTR_JR, "jr");
    line.addr = TEXT_START_ADDR + 5;
    line.args[0] = Token(SYM_LITERAL, -5, std::to_string(-5));
    line.sym_arg = 0;
    info.add(line);
    // else : ld c, a
    line.init();
    line.line_num = 16;  
    line.opcode = Token(SYM_INSTR, INSTR_LD, "ld");
    line.addr = TEXT_START_ADDR + 7;
    line.args[0] = Token(SYM_REG, REG_C, "c");
    line.args[1] = Token(SYM_REG, REG_A, "a");
    line.label = "else";
    line.is_label = true;
    info.add(line);
    // ld a, b
    line.init();
    line.line_num = 17;  
    line.opcode = Token(SYM_INSTR, INSTR_LD, "ld");
    line.addr = TEXT_START_ADDR + 8;
    line.args[0] = Token(SYM_REG, REG_A, "a");
    line.args[1] = Token(SYM_REG, REG_B, "b");
    info.add(line);
    // sub c 
    line.init();
    line.line_num = 18;  
    line.opcode = Token(SYM_INSTR, INSTR_SUB, "sub");
    line.addr = TEXT_START_ADDR + 9;
    line.args[0] = Token(SYM_REG, REG_C, "c");
    info.add(line);
    // ld b, a
    line.init();
    line.line_num = 19;  
    line.opcode = Token(SYM_INSTR, INSTR_LD, "ld");
    line.addr = TEXT_START_ADDR + 10;
    line.args[0] = Token(SYM_REG, REG_B, "b");
    line.args[1] = Token(SYM_REG, REG_A, "a");
    info.add(line);
    // ld a, c 
    line.init();
    line.line_num = 20;  
    line.opcode = Token(SYM_INSTR, INSTR_LD, "ld");
    line.addr = TEXT_START_ADDR + 11;
    line.args[0] = Token(SYM_REG, REG_A, "a");
    line.args[1] = Token(SYM_REG, REG_C, "c");
    info.add(line);
    // jr gcd
    line.init();
    line.line_num = 22;  
    line.opcode = Token(SYM_INSTR, INSTR_JR, "jr");
    line.addr = TEXT_START_ADDR + 12;
    line.args[0] = Token(SYM_LITERAL, -12, std::to_string(-12));
    line.sym_arg = 0;
    info.add(line);

    return info;
}


TEST_CASE("test_lex_gcd", "lexer")
{
    SourceInfo lex_source;
    SourceInfo exp_source;

    lex_source = lex_helper(gcd_filename);
    std::cout << "\t Lexer generated " << std::dec << lex_source.getNumLines() << " line of output" << std::endl;

    exp_source = get_gcd_expected_source();
    source_check_helper(exp_source, lex_source);
}

SourceInfo get_label_resolve_expected_source(void)
{
    SourceInfo info;
    LineInfo cur_line;

    // x: .defw 30
    cur_line.init();
    cur_line.type = LineType::DirectiveLine;
    cur_line.line_num = 5;
    cur_line.opcode = Token(SYM_DIRECTIVE, DIR_DEFW, ".defw");
    cur_line.addr = TEXT_START_ADDR;
    cur_line.label = "x";
    cur_line.is_label = true;
    cur_line.data = 30;       // NOTE: not resolved until assembly time, not ported back to info
    info.add(cur_line);
    // y: .defw 20
    cur_line.init();
    cur_line.type = LineType::DirectiveLine;
    cur_line.line_num = 6;
    cur_line.opcode = Token(SYM_DIRECTIVE, DIR_DEFW, ".defw");
    cur_line.addr = TEXT_START_ADDR + 2;
    cur_line.label = "y";
    cur_line.is_label = true;
    cur_line.data = 20;
    info.add(cur_line);
    // z: .defw -10 * (2 + x) - (3 * y)
    cur_line.init();
    cur_line.type = LineType::DirectiveLine;
    cur_line.line_num = 7;
    cur_line.opcode = Token(SYM_DIRECTIVE, DIR_DEFW, ".defw");
    cur_line.addr = TEXT_START_ADDR + 4;
    cur_line.label = "z";
    cur_line.is_label = true;
    cur_line.data = -380;
    info.add(cur_line);
    // ld hl (z)
    cur_line.init();
    cur_line.line_num = 9;
    cur_line.addr = TEXT_START_ADDR + 6;
    cur_line.opcode = Token(SYM_INSTR, INSTR_LD, "ld");
    cur_line.args[0] = Token(SYM_REG, REG_HL, "hl");
    cur_line.args[1] = Token(SYM_LITERAL, uint16_t(-380), "-380");
    info.add(cur_line);

    return info;
}

TEST_CASE("test_lex_label_resolve", "lexer")
{
    SourceInfo lex_source;
    SourceInfo exp_source;

    lex_source = lex_helper(label_resolve_filename);
    std::cout << "\t Lexer generated " << std::dec << lex_source.getNumLines() << " line of output" << std::endl;

    exp_source = get_label_resolve_expected_source();
    source_check_helper(exp_source, lex_source);
}

SourceInfo get_ret_lookahead_expected_source(void)
{
    SourceInfo info;
    LineInfo cur_line;

    // ret nz 
    cur_line.init();
    cur_line.line_num = 4;
    cur_line.opcode = Token(SYM_INSTR, INSTR_RET, "ret");
    cur_line.args[0] = Token(SYM_COND, COND_NZ, "nz");
    cur_line.addr = TEXT_START_ADDR;
    info.add(cur_line);
    // ret nc 
    cur_line.init();
    cur_line.line_num = 5;
    cur_line.opcode = Token(SYM_INSTR, INSTR_RET, "ret");
    cur_line.args[0] = Token(SYM_COND, COND_NC, "nc");
    cur_line.addr = TEXT_START_ADDR + 1;
    info.add(cur_line);
    // ret 
    cur_line.init();
    cur_line.line_num = 6;
    cur_line.opcode = Token(SYM_INSTR, INSTR_RET, "ret");
    cur_line.addr = TEXT_START_ADDR + 2;
    info.add(cur_line);
    // ret m 
    cur_line.init();
    cur_line.line_num = 7;
    cur_line.opcode = Token(SYM_INSTR, INSTR_RET, "ret");
    cur_line.args[0] = Token(SYM_COND, COND_M, "m");
    cur_line.addr = TEXT_START_ADDR + 3;
    info.add(cur_line);
    // ret  
    cur_line.init();
    cur_line.line_num = 9;
    cur_line.opcode = Token(SYM_INSTR, INSTR_RET, "ret");
    cur_line.addr = TEXT_START_ADDR + 4;
    info.add(cur_line);

    return info;
}

TEST_CASE("test_lex_ret_lookahead", "lexer")
{
    SourceInfo lex_source;
    SourceInfo exp_source;

    lex_source = lex_helper(ret_lookahead_filename);
    std::cout << "\t Lexer generated " << std::dec << lex_source.getNumLines() << " line of output" << std::endl;

    exp_source = get_ret_lookahead_expected_source();
    source_check_helper(exp_source, lex_source);
}

// ======== ASSEMBLER TESTS ======== //

Program get_add_sub_expected_program(void)
{
    Program prog;

    // ld a, 8  
    prog.add(Instr(TEXT_START_ADDR, 0x3E08, 2));
    // add a, 10
    prog.add(Instr(TEXT_START_ADDR + 2, 0xC60A, 2));
    // ld hl, $FC00 
    prog.add(Instr(TEXT_START_ADDR + 4, 0x21FC00, 3));
    // ld bc, $00BB
    prog.add(Instr(TEXT_START_ADDR + 7, 0x0100BB, 3));
    // ld b, 8
    prog.add(Instr(TEXT_START_ADDR + 10, 0x0608, 2));
    // ld a, b
    prog.add(Instr(TEXT_START_ADDR + 12, 0x78, 1));
    // add a, 5
    prog.add(Instr(TEXT_START_ADDR + 13, 0xC605, 2));
    // ld b,a 
    prog.add(Instr(TEXT_START_ADDR + 15, 0x47, 1));
    // ld bc, 46
    prog.add(Instr(TEXT_START_ADDR + 16, 0x01002E, 3));
    // ld h, b
    prog.add(Instr(TEXT_START_ADDR + 19, 0x60, 1));
    // ld l,c
    prog.add(Instr(TEXT_START_ADDR + 20, 0x69, 1));
    // ld bc, 52
    prog.add(Instr(TEXT_START_ADDR + 21, 0x010034, 3));
    // add hl, bc
    prog.add(Instr(TEXT_START_ADDR + 24, 0x09, 1));
    // ld b, h
    prog.add(Instr(TEXT_START_ADDR + 25, 0x44, 1));
    // ld c, l
    prog.add(Instr(TEXT_START_ADDR + 26, 0x4D, 1));

    return prog;
}


TEST_CASE("test_asm_add_sub", "assembler")
{
    int status;
    Assembler assem;
    SourceInfo lex_source;
    Program exp_program;
    Program out_program;

    assem.setVerbose(GLOBAL_VERBOSE);
    status = assem.read(add_sub_filename);
    REQUIRE(status == 0);
    assem.assemble();

    exp_program = get_add_sub_expected_program();
    out_program = assem.getProgram();

    std::cout << "Assembler produced " << std::dec << out_program.length() << " instructions" << std::endl;
    REQUIRE(exp_program.length() == out_program.length());

    program_check_helper(exp_program, out_program);
}


/*
 * GCD Program
 */
Program get_gcd_expected_program(void)
{
    Program prog;

    // gcd: cp b
    prog.add(Instr(TEXT_START_ADDR, 0xB8, 1));
    // ret z
    prog.add(Instr(TEXT_START_ADDR + 1, 0xC8, 1));
    // jr c, else
    prog.add(Instr(TEXT_START_ADDR + 2, 0x3803, 2));
    // sub b
    prog.add(Instr(TEXT_START_ADDR + 4, 0x90, 1));
    // jr gcd
    prog.add(Instr(TEXT_START_ADDR + 5, 0x1800 | uint8_t(-5), 2));
    //  else: ld c, a
    prog.add(Instr(TEXT_START_ADDR + 7, 0x4F, 1));
    // ld a, b
    prog.add(Instr(TEXT_START_ADDR + 8, 0x78, 1));
    // sub c
    prog.add(Instr(TEXT_START_ADDR + 9, 0x91, 1));
    // ld b, a
    prog.add(Instr(TEXT_START_ADDR + 10, 0x47, 1));
    // ld a, c
    prog.add(Instr(TEXT_START_ADDR + 11, 0x79, 1));
    // jr gcd
    prog.add(Instr(TEXT_START_ADDR + 12, 0x1800 | uint8_t(-12), 2));

    return prog;
}

TEST_CASE("test_asm_gcd", "assembler")
{
    int status;
    Assembler assem;
    SourceInfo lex_source;
    Program exp_program;
    Program out_program;

    assem.setVerbose(GLOBAL_VERBOSE);
    status = assem.read(gcd_filename);
    REQUIRE(status == 0);
    assem.assemble();

    exp_program = get_gcd_expected_program();
    out_program = assem.getProgram();

    std::cout << "Assembler produced " << std::dec << out_program.length() << " instructions" << std::endl;
    REQUIRE(exp_program.length() == out_program.length());
    program_check_helper(exp_program, out_program);
}

Program get_ret_lookahead_expected_program(void)
{
    Program prog;

    // ret nz
    prog.add(Instr(TEXT_START_ADDR, 0xC0, 1));
    // ret nc
    prog.add(Instr(TEXT_START_ADDR + 1, 0xD0, 1));
    // ret 
    prog.add(Instr(TEXT_START_ADDR + 2, 0xC9, 1));
    // ret m
    prog.add(Instr(TEXT_START_ADDR + 3, 0xF8, 1));
    // ret
    prog.add(Instr(TEXT_START_ADDR + 4, 0xC9, 1));

    return prog;
}

TEST_CASE("test_asm_ret_lookahead", "assembler")
{
    int status;
    Assembler assem;
    SourceInfo lex_source;
    Program exp_program;
    Program out_program;

    assem.setVerbose(GLOBAL_VERBOSE);
    status = assem.read(ret_lookahead_filename);
    REQUIRE(status == 0);
    assem.assemble();

    exp_program = get_ret_lookahead_expected_program();
    out_program = assem.getProgram();

    std::cout << "Assembler produced " << std::dec << out_program.length() << " instructions" << std::endl;
    REQUIRE(exp_program.length() == out_program.length());
    program_check_helper(exp_program, out_program);
}

// TEST_DIRECTIVES
TEST_CASE("test_equ_directive", "directive")
{
    const std::string test_source = " label: .equ 25";

    Assembler assem;
    Program out_program;

    assem.setVerbose(GLOBAL_VERBOSE);
    assem.loadSource(test_source);
    assem.assemble();
    out_program = assem.getProgram();

    REQUIRE(out_program.length() == 1);
    REQUIRE(out_program.get(0).ins == 25);
    REQUIRE(out_program.get(0).adr == TEXT_START_ADDR);
}


Program get_defw_expected_program(void)
{
    Program prog;

    prog.add(Instr(TEXT_START_ADDR, 16, 2));    // size should be 1 for defb

    return prog;
}   

SourceInfo get_defw_expected_source(void)
{
    SourceInfo info;
    LineInfo line;

    line.type = LineType::DirectiveLine;
    line.opcode = Token(SYM_DIRECTIVE, DIR_DEFW, ".defw");
    line.addr = TEXT_START_ADDR;
    line.line_num = 1;
    line.label = "label";
    line.is_label = true;
    line.data = 16;
    info.add(line);

    return info;
}


TEST_CASE("test_defw_single_literal", "directive")
{
    const std::string defw_literal_label = "label: .defw 16";

    Assembler assem;
    Program exp_program;
    Program out_program;
    SourceInfo exp_source;
    SourceInfo lex_source;
    LineInfo out_line;
    LineInfo exp_line;

    assem.setVerbose(GLOBAL_VERBOSE);
    assem.loadSource(defw_literal_label);
    assem.assemble();

    lex_source = assem.getSourceInfo();
    exp_source = get_defw_expected_source();

    REQUIRE(lex_source.getNumLines() == 1);
    //REQUIRE(lex_source.get(0) == exp_source.get(0));
    out_line = lex_source.get(0);
    out_line.data = 16;         // NOTE: we set here since at this time we don't do the eval until we are assembling the instruction (so its not available from the sourceinfo itself)
    exp_line = exp_source.get(0);

    REQUIRE(out_line == exp_line);
    out_program = assem.getProgram();
    exp_program = get_defw_expected_program();

    if(GLOBAL_VERBOSE)
    {
        std::cout << "out_program line : " << std::endl << out_program.get(0).toString() << std::endl;
        std::cout << "exp_program line : " << std::endl << exp_program.get(0).toString() << std::endl;
    }

    REQUIRE(out_program.get(0) == exp_program.get(0));
}

TEST_CASE("test_defw_literal_expr", "directive")
{
    const std::string defw_literal_expr = "label: .defw (8 * 4) / 2";

    Assembler assem;
    Program exp_program;
    Program out_program;
    SourceInfo exp_source;
    SourceInfo lex_source;
    LineInfo out_line;
    LineInfo exp_line;

    assem.setVerbose(GLOBAL_VERBOSE);
    assem.loadSource(defw_literal_expr);
    assem.assemble();

    lex_source = assem.getSourceInfo();
    exp_source = get_defw_expected_source();

    REQUIRE(lex_source.getNumLines() == 1);
    //REQUIRE(lex_source.get(0) == exp_source.get(0));

    out_line = lex_source.get(0);
    out_line.data = 16;         // NOTE: we set here since at this time we don't do the eval until we are assembling the instruction (so its not available from the sourceinfo itself)
    exp_line = exp_source.get(0);

    REQUIRE(out_line == exp_line);
    out_program = assem.getProgram();
    exp_program = get_defw_expected_program();

    if(GLOBAL_VERBOSE)
    {
        std::cout << "out_program line : " << std::endl << out_program.get(0).toString() << std::endl;
        std::cout << "exp_program line : " << std::endl << exp_program.get(0).toString() << std::endl;
    }

    REQUIRE(out_program.get(0) == exp_program.get(0));
}

TEST_CASE("test_org", "directive")
{
    const std::string org_fragment = ".org $BEEF";

    Assembler assem;
    assem.setVerbose(GLOBAL_VERBOSE);
    assem.loadSource(org_fragment);

    REQUIRE(assem.getCurAddr() == TEXT_START_ADDR);
    assem.assemble();
    REQUIRE(assem.getCurAddr() == 0xBEEF);
}


// ======== DIRECTIVES WITH EXPRESSIONS ======== //
SourceInfo get_expr_expected_source(void)
{
    SourceInfo info;
    LineInfo line;

    // scale: equ 256
    line.init();
    line.type = LineType::DirectiveLine;
    line.line_num = 4;  
    line.opcode = Token(SYM_DIRECTIVE, DIR_EQU, ".equ");
    line.addr = TEXT_START_ADDR;
    line.label = "scale";
    line.is_label = true;
    line.data = 256;
    info.add(line);

    // x: defw  2 * scale / 4
    line.init();
    line.type = LineType::DirectiveLine;
    line.line_num = 5;  
    line.opcode = Token(SYM_DIRECTIVE, DIR_DEFW, ".defw");
    line.addr = TEXT_START_ADDR + 2;
    line.label = "x";
    line.is_label = true;
    line.data = 128;
    info.add(line);

    // y: defw -5 * scale / 4
    line.init();
    line.type = LineType::DirectiveLine;
    line.line_num = 6;  
    line.opcode = Token(SYM_DIRECTIVE, DIR_DEFW, ".defw");
    line.addr = TEXT_START_ADDR + 4;
    line.label = "y";
    line.is_label = true;
    line.data = -320;
    info.add(line);

    // equation: .defw (2 * x) + y
    line.init();
    line.type = LineType::DirectiveLine;
    line.line_num = 8;  
    line.opcode = Token(SYM_DIRECTIVE, DIR_DEFW, ".defw");
    line.addr = TEXT_START_ADDR + 6;
    line.label = "equation";
    line.is_label = true;
    line.data = -64;
    info.add(line);

    return info;
}

Program get_expr_expected_program(void)
{
    Program prog;

    // scale: equ 256
    prog.add(Instr(TEXT_START_ADDR, 256, 2));
    // x: .defw 2 * scale / 4
    prog.add(Instr(TEXT_START_ADDR + 2, 128, 2));
    // y: defw -5 * scale / 4
    prog.add(Instr(TEXT_START_ADDR + 4, uint16_t(-320), 2));
    // equation: .defw (2 * x) + y
    prog.add(Instr(TEXT_START_ADDR + 6, uint16_t(-64), 2));

    return prog;
}

TEST_CASE("test_directive_expr", "expression")
{
    int status;
    Assembler assem;
    Program exp_program;
    Program out_program;
    SourceInfo exp_source;
    SourceInfo lex_source;
    LineInfo out_line;
    LineInfo exp_line;

    assem.setVerbose(GLOBAL_VERBOSE);
    status = assem.read(expr_filename);
    REQUIRE(status == 0);
    assem.assemble();

    lex_source = assem.getSourceInfo();
    exp_source = get_expr_expected_source();

    REQUIRE(lex_source.getNumLines() == exp_source.getNumLines());

    source_check_helper(exp_source, lex_source);

    out_program = assem.getProgram();
    exp_program = get_expr_expected_program();

    REQUIRE(out_program.length() == exp_program.length());
    program_check_helper(exp_program, out_program);
}


