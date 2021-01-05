/*
 * UTIL
 * Stuff that doesn't fit anywhere else
 */

#ifndef __UTIL_HPP
#define __UTIL_HPP

#include <cstdint>
#include <string>
#include <vector>

/*
 * Generate random array of bytes
 */
std::vector<uint8_t> gen_random_byte_array(const int size);

/*
 * Read a memory dump file into a vector
 */
std::vector<uint8_t> read_memory(const std::string& filename);


#endif /*__UTIL_HPP*/
