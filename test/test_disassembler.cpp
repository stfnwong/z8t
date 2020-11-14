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


TEST_CASE("test_code_buffer_sample", "[classic]")
{
    Assembler assem;
    Program out_prog;
    SourceInfo out_source;

    const std::string src_fragment = "ld a, 20\nld b, 20\nadd a,b";

    assem.setVerbose(GLOBAL_VERBOSE);
    assem.loadSource(src_fragment);
    assem.assemble();

    out_prog = assem.getProgram();



}
