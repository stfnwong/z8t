/*
 * TEST_PROGRAM
 * Tests for objects in Program.cpp
 */


#define CATCH_CONFIG_MAIN
#include "catch/catch.hpp"

#include <iostream> 
#include <iomanip>
#include <vector>
#include <string>

#include "Assembler.hpp"
#include "Program.hpp"

constexpr const bool GLOBAL_VERBOSE = false;


TEST_CASE("test_src_program_read_write", "program")
{
    Assembler assem;
    Program src_prog;
    Program dst_prog;

    const std::string prog_file = "test.prog";
    const std::string instr_literal = "ld a, b";

    assem.setVerbose(GLOBAL_VERBOSE);
    assem.loadSource(instr_literal);
    assem.assemble();

    src_prog = assem.getProgram();

    //std::cout << "Output src_program is :" << std::endl;
    //std::cout << src_prog.toString() << std::endl;
   
    REQUIRE(src_prog.length() == 1);
    src_prog.save(prog_file);

    dst_prog.load(prog_file);
    REQUIRE(src_prog.length() == dst_prog.length());

    std::cout << "src_prog : " << std::endl;
    std::cout << src_prog.toString() << std::endl;

    std::cout << "dst_prog : " << std::endl;
    std::cout << dst_prog.toString() << std::endl;

    REQUIRE(src_prog == dst_prog);
}


TEST_CASE("test_to_array", "program")
{
    Assembler assem;
    Program prog;
    const std::string src_fragment = "ld a, 20\nld b, 20\nadd a,b";
    const std::vector<uint8_t> expected_vector = {
        0x3E, 0x14,     // ld a, 20
        0x06, 0x14,     // ld b, 20
        0x80            // add a,b
    };

    assem.setVerbose(GLOBAL_VERBOSE);
    assem.loadSource(src_fragment);
    assem.assemble();

    prog = assem.getProgram();
    std::vector<uint8_t> prog_vector = prog.toArray();

    REQUIRE(prog_vector.size() == expected_vector.size());
    for(unsigned int idx = 0; idx < expected_vector.size(); ++idx)
    {
        REQUIRE(prog_vector[idx] == expected_vector[idx]);
    }
}


//TEST_CASE("test_program_end_to_end", "program")
//{
//}
