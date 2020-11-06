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

constexpr const bool GLOBAL_VERBOSE = true;


TEST_CASE("test_src_program_read_write", "[classic]")
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
