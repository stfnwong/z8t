/*
 * UTIL
 * Things that don't fit anywhere else
 */

#ifndef __UTIL_HPP
#define __UTIL_HPP

#include <cstdint>
#include <string>
#include <vector>
#include <cmath>

#define COMP_EPSILON 1e-5

/*
 * Generate random array of bytes
 */
std::vector<uint8_t> gen_random_byte_array(const int size);

/*
 * Read a memory dump file into a vector
 */
std::vector<uint8_t> read_memory(const std::string& filename);

/*
 * simple floating point equality test
 */
inline bool equal(float a, float b)
{
    return (std::fabs(a - b) < COMP_EPSILON) ? true : false;
}


#endif /*__UTIL_HPP*/
