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

constexpr const bool GLOBAL_VERBOSE = true;


TEST_CASE("test_cpu_state_init", "[cpu]")
{
    CPUState state;

    REQUIRE(state.pc == 0);
    REQUIRE(state.sp == 0);
    REQUIRE(state.ix == 0);
    REQUIRE(state.iy == 0);

    REQUIRE(state.acc == 0);
    REQUIRE(state.flags == 0);

    REQUIRE(state.bc == 0);
    REQUIRE(state.de == 0);
    REQUIRE(state.hl == 0);
    REQUIRE(state.wz == 0);
}

TEST_CASE("test_8bit_read_write", "[cpu]")
{
    CPUState state;

    REQUIRE(state.read_b() == uint8_t(0));
    state.write_b(8);
    REQUIRE(state.read_b() == uint8_t(8));

    REQUIRE(state.read_c() == uint8_t(0));
    state.write_c(10);
    REQUIRE(state.read_c() == uint8_t(10));
}

TEST_CASE("test_16bit_read_write", "[cpu]")
{
    CPUState state;

    REQUIRE(state.read_bc() == uint16_t(0));

    state.write_bc(512);
    REQUIRE(state.read_bc() == uint16_t(512));
    state.write_c(10);
    REQUIRE(state.read_c() == uint16_t(10));
    REQUIRE(state.read_bc() == uint16_t((512 & 0xFF00) | 10));
    state.write_b(5);
    REQUIRE(state.read_b() == uint8_t(5));
    REQUIRE(state.read_bc() == uint16_t((5 << 8) | 10));
}
