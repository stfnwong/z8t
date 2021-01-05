/*
 * UTIL
 * Stuff that doesn't fit anywhere else
 */

#include <cstdlib>
#include <fstream>
#include "Util.hpp"


/*
 * Generate random array of bytes
 */
std::vector<uint8_t> gen_random_byte_array(const int size)
{
    std::vector<uint8_t> byte_vec(size);

    for(unsigned int i = 0; i < byte_vec.size(); ++i)
        byte_vec[i] = rand();

    return byte_vec;
}

/*
 * read_memory()
 */
std::vector<uint8_t> read_memory(const std::string& filename)
{
    unsigned int len;
    std::ifstream infile(filename, std::ios::binary);

    // do size check 
    infile.seekg(0, std::ios::end);
    len = infile.tellg();
    infile.seekg(0, std::ios::beg);

    std::vector<uint8_t> mem_array(len);

    //infile.read(this->data, len);
    uint8_t byte;
    for(unsigned int b = 0; b < len; ++b)
    {
        infile.read(reinterpret_cast<char*>(&byte), sizeof(uint8_t));
        mem_array[b] = byte;
    }

    return mem_array;
}
