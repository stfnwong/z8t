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

constexpr const bool GLOBAL_VERBOSE = true;
const std::string add_sub_filename = "asm/add_sub.asm";
const std::string indirect_filename = "asm/indirect_test.asm";
const std::string gcd_filename = "asm/gcd.asm";

// TODO: a test which goes through the entire lookup table and checks
// that all instructions return valid pairs
//TEST_CASE("", "")
//{
//
//}


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

// ======== LEXER TESTS ======== //

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


TEST_CASE("test_lex_add_sub", "[classic]")
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


TEST_CASE("test_lex_indirect", "[classic]")
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

/*
 * GCD source
 */
SourceInfo get_gcd_expected_source(void)
{
    SourceInfo info;
    TextLine line;

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
    line.args[1] = Token(SYM_LITERAL, 0x1, "1");        
    // TODO: this should be 3 since we jump over "sub b" and "jr gcd"
    //line.args[1] = Token(SYM_LITERAL, 0x3, "3");        
    line.symbol = "else";
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
    info.add(line);
    // else : ld c, a
    line.init();
    line.line_num = 16;  
    line.opcode = Token(SYM_INSTR, INSTR_LD, "ld");
    line.addr = TEXT_START_ADDR + 7;
    line.args[0] = Token(SYM_LITERAL, TEXT_START_ADDR, std::to_string(TEXT_START_ADDR));
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
    line.args[1] = Token(SYM_REG, REG_C, "c");
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
    line.args[0] = Token(SYM_LITERAL, TEXT_START_ADDR, std::to_string(TEXT_START_ADDR));
    line.sym_arg = 0;
    info.add(line);

    return info;
}


TEST_CASE("test_lex_gcd", "[classic]")
{
    SourceInfo lex_source;
    SourceInfo exp_source;

    lex_source = lex_helper(gcd_filename);
    std::cout << "\t Lexer generated " << lex_source.getNumLines() << " line of output" << std::endl;

    exp_source = get_gcd_expected_source();
    // TODO : debug, remove 
    for(unsigned int i = 0; i < lex_source.getNumLines(); ++i)
    {
        TextLine cur_line = lex_source.get(i);
        std::cout << cur_line.toString() << std::endl;
    }

    // Check intermediate results
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

// ======== ASSEMBLER TESTS ======== //

Program get_add_sub_expected_program(void)
{
    Program prog;
    Instr cur_instr;

    // ld a, 8  
    cur_instr.init();
    cur_instr.ins = 0x3E08;
    cur_instr.size = 2;
    cur_instr.adr = TEXT_START_ADDR;
    prog.add(cur_instr);
    // add a, 10
    cur_instr.init();
    cur_instr.ins = 0xC60A;
    cur_instr.size = 2;
    cur_instr.adr = TEXT_START_ADDR + 2;
    prog.add(cur_instr);
    // ld hl, $FC00 
    cur_instr.init();
    cur_instr.ins = 0x21FC00;
    cur_instr.size = 3;
    cur_instr.adr = TEXT_START_ADDR + 4;
    prog.add(cur_instr);
    // ld bc, $00BB
    cur_instr.init();
    cur_instr.ins = 0x0100BB;
    cur_instr.size = 3;
    cur_instr.adr = TEXT_START_ADDR + 7;
    prog.add(cur_instr);
    // ld b, 8
    cur_instr.init();
    cur_instr.ins = 0x0608;
    cur_instr.size = 2;
    cur_instr.adr = TEXT_START_ADDR + 10;
    prog.add(cur_instr);
    // ld a, b
    cur_instr.init();
    cur_instr.ins = 0x78;
    cur_instr.size = 1;
    cur_instr.adr = TEXT_START_ADDR + 12;
    prog.add(cur_instr);
    // add a, 5
    cur_instr.init();
    cur_instr.ins = 0xC605;
    cur_instr.size = 2;
    cur_instr.adr = TEXT_START_ADDR + 13;
    prog.add(cur_instr);
    // ld b,a 
    cur_instr.init();
    cur_instr.ins = 0x06;
    cur_instr.size = 1;
    cur_instr.adr = TEXT_START_ADDR + 15;
    prog.add(cur_instr);
    // ld bc, 46
    cur_instr.init();
    cur_instr.ins = 0x01002E;
    cur_instr.size = 3;
    cur_instr.adr = TEXT_START_ADDR + 16;
    prog.add(cur_instr);
    // ld h, b
    cur_instr.init();
    cur_instr.ins = 0x60;
    cur_instr.size = 1;
    cur_instr.adr = TEXT_START_ADDR + 19;
    prog.add(cur_instr);
    // ld l,c
    cur_instr.init();
    cur_instr.ins = 0x69;
    cur_instr.size = 1;
    cur_instr.adr = TEXT_START_ADDR + 20;
    prog.add(cur_instr);
    // ld bc, 52
    cur_instr.init();
    cur_instr.ins = 0x010034;
    cur_instr.size = 3;
    cur_instr.adr = TEXT_START_ADDR + 21;
    prog.add(cur_instr);
    // add hl, bc
    cur_instr.init();
    cur_instr.ins = 0x09;
    cur_instr.size = 1;
    cur_instr.adr = TEXT_START_ADDR + 24;
    prog.add(cur_instr);
    // ld b, h
    cur_instr.init();
    cur_instr.ins = 0x44;
    cur_instr.size = 1;
    cur_instr.adr = TEXT_START_ADDR + 25;
    prog.add(cur_instr);
    // ld c, l
    cur_instr.init();
    cur_instr.ins = 0x4D;
    cur_instr.size = 1;
    cur_instr.adr = TEXT_START_ADDR + 26;
    prog.add(cur_instr);

    return prog;
}


TEST_CASE("test_asm_add_sub", "[classic]")
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

    // TODO: debug, remove 
    //for(unsigned int idx = 0; idx < out_program.length(); ++idx)
    //{
    //    Instr instr = out_program.get(idx);
    //    std::cout << "(" << idx + 1 << ")" << instr.toString() << std::endl;
    //}

    std::cout << "Assembler produced " << out_program.length() << " instructions" << std::endl;
    REQUIRE(exp_program.length() == out_program.length());

    for(unsigned int idx = 0; idx < out_program.length(); ++idx)
    {
        Instr out_instr = out_program.get(idx);
        Instr exp_instr = exp_program.get(idx);

        if(out_instr != exp_instr)
        {
            std::cout << "Difference in instruction " << idx + 1 << std::endl;
            std::cout << "Expected : " << exp_instr.toString() << std::endl;
            std::cout << "Got      : " << out_instr.toString() << std::endl;
        }
        REQUIRE(out_instr == exp_instr);
    }
}


/*
 * GCD Program
 */
Program get_gcd_expected_program(void)
{
    Program prog;
    Instr cur_instr;

    // gcd: cp b
    cur_instr.init();
    cur_instr.ins = 0xB8;
    cur_instr.size = 1;
    cur_instr.adr = TEXT_START_ADDR;
    prog.add(cur_instr);
    // ret z
    cur_instr.init();
    cur_instr.ins = 0xC8;
    cur_instr.size = 1;
    cur_instr.adr = TEXT_START_ADDR + 1;
    prog.add(cur_instr);
    // jr c, else
    cur_instr.init();
    cur_instr.ins = 0x3803;     // offset AFTER PC is incremented
    cur_instr.size = 2;
    cur_instr.adr = TEXT_START_ADDR + 2;
    prog.add(cur_instr);
    // sub b
    cur_instr.init();
    cur_instr.ins = 0x90;
    cur_instr.size = 1;
    cur_instr.adr = TEXT_START_ADDR + 4;
    prog.add(cur_instr);
    // jr gcd
    cur_instr.init();
    cur_instr.ins = 0x1800;
    cur_instr.size = 2;
    cur_instr.adr = TEXT_START_ADDR + 6;
    prog.add(cur_instr);
    //  else: ld c, a
    cur_instr.init();
    cur_instr.ins = 0x1800;
    cur_instr.size = 2;
    cur_instr.adr = TEXT_START_ADDR + 6;
    prog.add(cur_instr);
    // ld a, b


    return prog;
}

TEST_CASE("test_asm_gcd", "[classic]")
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
    //REQUIRE(exp_program.length() == out_program.length());

    for(unsigned int idx = 0; idx < out_program.length(); ++idx)
    {
        Instr out_instr = out_program.get(idx);
        Instr exp_instr = exp_program.get(idx);

        if(out_instr != exp_instr)
        {
            std::cout << "Difference in instruction " << idx + 1 << std::endl;
            std::cout << "Expected : " << exp_instr.toString() << std::endl;
            std::cout << "Got      : " << out_instr.toString() << std::endl;
        }
        REQUIRE(out_instr == exp_instr);
    }
}
