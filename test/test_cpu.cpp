/* TEST_CPU 
 * Tests for Z80 CPU state objects
 *
 * Stefan Wong 2020
 */

#define CATCH_CONFIG_MAIN
#include "catch/catch.hpp"

#include <iostream> 
#include <iomanip>
#include <vector>
#include <string>

#include "CPU.hpp"
#include "Util.hpp"

constexpr const bool GLOBAL_VERBOSE = true;
constexpr unsigned int test_mem_size = 0x10000;

// ======== MEMORY ======== //
TEST_CASE("test_memory_init", "memory")
{
    Memory mem(test_mem_size);
    
    REQUIRE(mem.size() == test_mem_size); 
    mem.clear();
    for(unsigned int i = 0; i < mem.size(); ++i)
        REQUIRE(mem[i] == 0);
}

TEST_CASE("test_memory_read_write", "memory")
{
    Memory mem(test_mem_size);

    for(unsigned int i = 0; i < 32; ++i)
        mem[i] = 2 * i;

    for(unsigned int i = 0; i < 32; ++i)
        REQUIRE(mem[i] == 2 * i);

    mem.clear();
    for(unsigned int i = 0; i < 32; ++i)
        REQUIRE(mem[i] == 0);
}

TEST_CASE("test_memory_load", "memory")
{
    Memory mem(test_mem_size);

    // create some dummy data
    unsigned int test_data_size = 64;
    uint8_t* test_data = new uint8_t[test_data_size];

    for(unsigned int i = 0; i < test_data_size; ++i)
        test_data[i] = 2 * i;

    mem.load(test_data, test_data_size, 0);
    for(unsigned int i = 0; i < test_data_size; ++i)
        REQUIRE(mem[i] == test_data[i]);

    mem.load(test_data, test_data_size, 2 * test_data_size);
    for(unsigned int i = (2* test_data_size); i < (2 * test_data_size) + test_data_size; ++i)
        REQUIRE(mem[i] == test_data[i - (2 * test_data_size)]);

    delete[] test_data;
}

TEST_CASE("test_memory_load_save", "memory")
{
    Memory mem(test_mem_size);

    // create some random data to place into memory 
    std::vector<uint8_t> test_mem_data = gen_random_byte_array(test_mem_size >> 1);
    REQUIRE(test_mem_data.size() == test_mem_size >> 1);

    mem.load(test_mem_data, test_mem_data.size(), 0);
    // Check the data loaded correctly
    for(unsigned int b = 0; b < test_mem_data.size(); ++b)
        REQUIRE(mem[b] == test_mem_data[b]);

    // Write memory contents to file 
    const std::string filename = "memory_load_test.dat";
    mem.save(filename);

    // read the file into a new memory object and check
    Memory out_mem(test_mem_size);
    out_mem.load(filename, 0);

    for(unsigned int b = 0; b < test_mem_data.size(); ++b)
        REQUIRE(mem[b] == out_mem[b]);
}


// ======== CPU ======== //
TEST_CASE("test_cpu_state_init", "cpu")
{
    CPUState state;

    REQUIRE(state.pc == 0);
    REQUIRE(state.sp == 0);
    REQUIRE(state.ix == 0);
    REQUIRE(state.iy == 0);

    for(int f = 0; f < 8; ++f)
        REQUIRE(state.flags[f] == false);

    // general registers 
    REQUIRE(state.a == 0);
    REQUIRE(state.b == 0);
    REQUIRE(state.c == 0);
    REQUIRE(state.d == 0);
    REQUIRE(state.e == 0);
    REQUIRE(state.h == 0);
    REQUIRE(state.l == 0);
    REQUIRE(state.w == 0);
    REQUIRE(state.z == 0);

    REQUIRE(state.mem.size() == 0x10000);
}

TEST_CASE("test_16bit_read_write", "cpu")
{
    CPUState state;

    REQUIRE(state.h == 0);
    REQUIRE(state.l == 0);
    REQUIRE(state.read_hl() == 0);

    // BC
    state.b = 0xBB;
    REQUIRE(state.read_bc() == 0xBB00);
    state.write_bc(0xCCBB);
    REQUIRE(state.b == 0xCC);
    REQUIRE(state.c == 0xBB);

    // DE
    state.d = 0xD0;
    state.e = 0xE0;
    REQUIRE(state.read_de() == 0xD0E0);
    state.write_de(0xF0F0);
    REQUIRE(state.d == 0xF0);
    REQUIRE(state.e == 0xF0);

    // HL
    state.h = 0xFF;
    REQUIRE(state.read_hl() == 0xFF00);
    state.l = 0xAA;
    REQUIRE(state.read_hl() == 0xFFAA);
}


// Test the opcode fetch machine cycle
TEST_CASE("test_opcode_fetch_cycle", "cpu")
{
    CPUState state;

    // test instruction 
    unsigned int test_addr = 0x2002;
    uint16_t     test_machine_code = 0x47;      // LD B, A
    state.mem[test_addr] = test_machine_code;

    // dummy data
    state.a = 0xDE;
    REQUIRE(state.b == 0);
    // set the pc to start at the test address
    state.pc = test_addr;

    state.fetch();
    REQUIRE(state.data_bus == test_machine_code);
    REQUIRE(state.pc == test_addr + 1);
    //state.decode();
    state.exec_opcode();
    REQUIRE(state.b == state.a);
}
