/* 
 * TEST_LOOKUP 
 *
 * Stefan Wong 2018
 */

#define CATCH_CONFIG_MAIN
#include "catch/catch.hpp"

#include <iostream> 
#include <iomanip>
#include <vector>
#include <string>

#include "Lookup.hpp"
#include "Register.hpp"




TEST_CASE("test_reg_map_init", "[classic]")
{
    Lookup<Register> reg_map(Z80_REGISTERS);


}
