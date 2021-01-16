/*
 * TEST_EXEC
 * Test Program execution in CPU emulator
 */

#define CATCH_CONFIG_MAIN
#include "catch/catch.hpp"

#include <iostream> 
#include <iomanip>
#include <vector>
#include <string>

#include "Assembler.hpp"
#include "CPU.hpp"
#include "Program.hpp"

constexpr const bool GLOBAL_VERBOSE = false;
const std::string gcd_filename = "asm/gcd_run.asm";



TEST_CASE("test_exec_gcd", "execute")
{
    int status;
    Assembler assem;
    Program out_program;
    CPUState state;

    // assemble the program
    assem.setVerbose(GLOBAL_VERBOSE);
    status = assem.read(gcd_filename);
    REQUIRE(status == 0);
    assem.assemble();

    out_program = assem.getProgram();
    std::vector<uint8_t> prog_bytes = out_program.toArray();

    // TODO: debug only, remove 
    std::cout << out_program.toString() << std::endl;

    state.mem.load(prog_bytes, prog_bytes.size(), out_program.startAddr());
    // TODO: this test can go later, its just to convince me the load() worked 
    for(unsigned int i = 0; i < prog_bytes.size(); ++i)
        REQUIRE(state.mem[i] == prog_bytes[i]);

    //  start cycling the machine 
}
