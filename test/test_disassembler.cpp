/*
 * TEST_DISASSEMBLER
 * Tests for Disassembler methods
 */


#define CATCH_CONFIG_MAIN
#include "catch/catch.hpp"

#include <iostream> 
#include <iomanip>
#include <vector>
#include <string>

#include "Assembler.hpp"
#include "Disassembler.hpp"
#include "Source.hpp"
#include "Program.hpp"

constexpr const bool GLOBAL_VERBOSE = true;


TEST_CASE("test_dis_instr_to_string", "[classic]")
{
    Assembler assem;
    Program out_prog;
    SourceInfo out_source;

    const std::string src_fragment = "ld a, 20\nld b, 20\nadd a,b";
    const std::vector<uint8_t> expected_vector = {
        0x3E, 0x14,     // ld a, 20
        0x06, 0x14,     // ld b, 20
        0x80            // add a,b
    };

    assem.setVerbose(GLOBAL_VERBOSE);
    assem.loadSource(src_fragment);
    assem.assemble();

    out_prog = assem.getProgram();
    std::vector<uint8_t> prog_vector = out_prog.toArray();
    REQUIRE(expected_vector.size() == prog_vector.size());

    for(unsigned int byte = 0; byte < expected_vector.size(); ++byte)
    {
        REQUIRE(expected_vector[byte] == prog_vector[byte]);
    }

    // TODO: write expected string
    std::string dis_string = dis_instr_to_repr(prog_vector);
    std::cout << dis_string << std::endl;
}

TEST_CASE("test_dis_instr_to_program", "[classic]")
{
    Assembler assem;
    Program asm_prog;
    Program dis_prog;

    const std::string src_fragment = "ld a, 20\nld b, 20\nadd a,b";
    const std::vector<uint8_t> expected_vector = {
        0x3E, 0x14,     // ld a, 20
        0x06, 0x14,     // ld b, 20
        0x80            // add a,b
    };

    assem.setVerbose(GLOBAL_VERBOSE);
    assem.loadSource(src_fragment);
    assem.assemble();
    asm_prog = assem.getProgram();
    std::vector<uint8_t> asm_prog_vector = asm_prog.toArray();

    dis_prog = dis_instr_to_program(asm_prog_vector, 0x1000);

    REQUIRE(dis_prog.length() == asm_prog.length());
    for(unsigned int idx = 0; idx < asm_prog.length(); ++idx)
    {
        Instr dis_instr = dis_prog.get(idx);
        Instr asm_instr = asm_prog.get(idx);

        if(dis_instr != asm_instr)
        {
            std::cout << "Error in instruction [" << std::dec << idx << "/" << asm_prog.length() << "]" << std::endl;
            std::cout << "Expected :" << asm_instr.toString() << std::endl;
            std::cout << "Got      :" << dis_instr.toString() << std::endl;
        }

        REQUIRE(dis_instr == asm_instr);
    }
}
