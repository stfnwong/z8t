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


TEST_CASE("test_to_array", "[classic]")
{
    Assembler assem;
    Program prog;
    const std::string filename = "asm/add_sub.asm";
    const std::string src_fragment = "ld a, 20\nld b, 20\nadd a,b";
    const std::vector<uint8_t> expected_vector = {
        0x3E, 0x14,     // ld a, 20
        0x06, 0x14,     // ld b, 20
        0x80            // add a,b
    };

    // TODO: move to helper function later 
    assem.setVerbose(GLOBAL_VERBOSE);
    assem.loadSource(src_fragment);
    //std::cout << "\t Reading file " << filename << std::endl;
    //status = assem.read(filename);
    //if(status != 0)
    //{
    //    std::cout << "[" << __func__ << "] failed to read file " << filename << std::endl;
    //    return info;
    //}
    //
    //std::cout << "\t Lexing file " << filename << std::endl;
    assem.assemble();

    prog = assem.getProgram();
    std::cout << "Program string : " << std::endl;
    std::cout << prog.toString() << std::endl;
    std::vector<uint8_t> prog_vector = prog.toArray();

    std::cout << std::dec << prog_vector.size() << " bytes in output program" << std::endl;
    for(unsigned int byte = 0; byte < prog_vector.size(); ++byte)
    {
        std::cout << std::setw(2) << std::setfill('0') << std::hex << unsigned(prog_vector[byte]) << "h ";
    }
    std::cout << std::endl;

    REQUIRE(prog_vector.size() == expected_vector.size());
    for(unsigned int idx = 0; idx < expected_vector.size(); ++idx)
    {
        REQUIRE(prog_vector[idx] == expected_vector[idx]);
    }

}

//TEST_CASE("test_program_end_to_end", "[classic]")
//{
//}
