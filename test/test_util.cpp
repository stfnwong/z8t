/* TEST_UTIL 
 * Unit tests for util functions
 *
 */

#define CATCH_CONFIG_MAIN
#include "catch/catch.hpp"

#include <iostream>
#include <iomanip>
#include <vector>

#include "Util.hpp"


TEST_CASE("test_gen_random_bytes", "util")
{
    std::vector<uint8_t> byte_vec = gen_random_byte_array(10);
    REQUIRE(byte_vec.size() == 10);

    for(unsigned int i = 0; i < byte_vec.size(); ++i)
        std::cout << "[" << std::setw(4) << i << "] : " << unsigned(byte_vec[i]) << std::endl;
}

