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
#include "Lexer.hpp"
#include "Source.hpp"
#include "Program.hpp"

constexpr const bool GLOBAL_VERBOSE = true;
const std::string add_sub_filename = "asm/add_sub.asm";

// TODO : fold in lexer test cases to the top of this file


// TODO: addresses are wrong
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
    cur_instr.adr = TEXT_START_ADDR + 1;
    prog.add(cur_instr);
    // ld hl, $FC00 
    cur_instr.init();
    cur_instr.ins = 0x21FC00;
    cur_instr.size = 3;
    cur_instr.adr = TEXT_START_ADDR + 2;
    prog.add(cur_instr);
    // ld bc, $00BB
    cur_instr.init();
    cur_instr.ins = 0x0100BB;
    cur_instr.size = 3;
    cur_instr.adr = TEXT_START_ADDR + 3;
    prog.add(cur_instr);
    // ld b, 8
    cur_instr.init();
    cur_instr.ins = 0x0608;
    cur_instr.size = 2;
    cur_instr.adr = TEXT_START_ADDR + 4;
    prog.add(cur_instr);
    // ld a, b
    cur_instr.init();
    cur_instr.ins = 0x78;
    cur_instr.size = 1;
    cur_instr.adr = TEXT_START_ADDR + 5;
    prog.add(cur_instr);
    // add a, 5
    cur_instr.init();
    cur_instr.ins = 0xC605;
    cur_instr.size = 2;
    cur_instr.adr = TEXT_START_ADDR + 6;
    prog.add(cur_instr);
    // ld b,a 
    cur_instr.init();
    cur_instr.ins = 0x06;
    cur_instr.size = 1;
    cur_instr.adr = TEXT_START_ADDR + 7;
    prog.add(cur_instr);
    // ld bc, 46
    cur_instr.init();
    cur_instr.ins = 0x01002E;
    cur_instr.size = 3;
    cur_instr.adr = TEXT_START_ADDR + 8;
    prog.add(cur_instr);
    // ld h, b
    cur_instr.init();
    cur_instr.ins = 0x60;
    cur_instr.size = 1;
    cur_instr.adr = TEXT_START_ADDR + 9;
    prog.add(cur_instr);
    // ld l,c
    cur_instr.init();
    cur_instr.ins = 0x69;
    cur_instr.size = 1;
    cur_instr.adr = TEXT_START_ADDR + 10;
    prog.add(cur_instr);
    // ld bc, 52
    cur_instr.init();
    cur_instr.ins = 0x010034;
    cur_instr.size = 3;
    cur_instr.adr = TEXT_START_ADDR + 11;
    prog.add(cur_instr);
    // add hl, bc
    cur_instr.init();
    cur_instr.ins = 0x09;
    cur_instr.size = 1;
    cur_instr.adr = TEXT_START_ADDR + 12;
    prog.add(cur_instr);
    // ld b, h
    cur_instr.init();
    cur_instr.ins = 0x44;
    cur_instr.size = 1;
    cur_instr.adr = TEXT_START_ADDR + 13;
    prog.add(cur_instr);
    // ld c, l
    cur_instr.init();
    cur_instr.ins = 0x4D;
    cur_instr.size = 1;
    cur_instr.adr = TEXT_START_ADDR + 14;
    prog.add(cur_instr);

    return prog;
}


TEST_CASE("test_add_sub", "[classic]")
{
    Assembler assem;
    SourceInfo lex_source;
    Program exp_program;
    Program out_program;

    assem.setVerbose(GLOBAL_VERBOSE);
    assem.loadSource(add_sub_filename);
    assem.assemble();

    exp_program = get_add_sub_expected_program();
    out_program = assem.getProgram();

    REQUIRE(exp_program.numInstr() == out_program.numInstr());

    for(unsigned int idx = 0; idx < out_program.numInstr(); ++idx)
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


// TODO: will need to change all the addresses...
/*
 * GCD Program
 */
Program get_gcd_expected_program(void)
{
    Program prog;
    Instr cur_instr;

    // cp b
    cur_instr.init();
    cur_instr.ins = 0xB8;
    cur_instr.size = 1;
    cur_instr.adr = TEXT_START_ADDR;
    prog.add(cur_instr);
    // ret z
    cur_instr.init();
    cur_instr.ins = 0xC8;
    cur_instr.size = 1;
    cur_instr.adr = TEXT_START_ADDR + 2;
    prog.add(cur_instr);
    // jr c, else
    cur_instr.init();
    cur_instr.ins = 0x3800;
    cur_instr.size = 1;
    cur_instr.adr = TEXT_START_ADDR + 3;
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
    cur_instr.adr = TEXT_START_ADDR + 5;
    prog.add(cur_instr);


    return prog;
}
