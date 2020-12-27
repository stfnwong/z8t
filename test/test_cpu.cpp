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

// Test all register reads and writes
TEST_CASE("test_16bit_read_write", "[cpu]")
{
    CPUState state;

    REQUIRE(state.read_bc() == uint16_t(0));
    REQUIRE(state.read_de() == uint16_t(0));
    REQUIRE(state.read_hl() == uint16_t(0));
    REQUIRE(state.read_wz() == uint16_t(0));

    // do something with bc
    state.write_bc(512);
    REQUIRE(state.read_bc() == uint16_t(512));
    state.write_c(10);
    REQUIRE(state.read_c() == uint16_t(10));
    REQUIRE(state.read_bc() == uint16_t((512 & 0xFF00) | 10));
    state.write_b(5);
    REQUIRE(state.read_b() == uint8_t(5));
    REQUIRE(state.read_bc() == uint16_t((5 << 8) | 10));

    // do something with de
    state.write_d(128);
    REQUIRE(state.read_d() == uint8_t(128));
    state.write_e(128);
    REQUIRE(state.read_e() == uint8_t(128));
    REQUIRE(state.read_de() == uint16_t((128 << 8) | 128));
    state.write_d(80);
    REQUIRE(state.read_de() == uint16_t((80 << 8) | 128));

    // do something with hl
    state.write_h(50);
    REQUIRE(state.read_h() == uint8_t(50));
    state.write_l(22);
    REQUIRE(state.read_l() == uint8_t(22));
    REQUIRE(state.read_hl() == uint16_t(((50 << 8) | 22)));
    state.write_hl(600);
    REQUIRE(state.read_h() == uint8_t((600 & 0xFF00) >> 8));
    REQUIRE(state.read_l() == uint8_t(600 & 0x00FF));

    // do something with wz
    state.write_w(32);
    REQUIRE(state.read_w() == uint8_t(32));
    REQUIRE(state.read_wz() == uint16_t(32 << 8));
    state.write_z(64);
    REQUIRE(state.read_z() == uint8_t(64));
    REQUIRE(state.read_wz() == uint16_t((32 << 8) | 64));
    state.write_wz(8);
    REQUIRE(state.read_wz() == 8);
    REQUIRE(state.read_w() == 0);
    REQUIRE(state.read_z() == 8);
}
