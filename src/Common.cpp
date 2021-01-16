/*
 * COMMON
 * Stuff that gets used everywhere
 */

#include "Common.hpp"

#include <iomanip>


void error_general(const int line_num, const int addr, const std::string& errstr) 
{
    std::cout << "Error on line " << std::dec << line_num << " [0x" 
        << std::hex << std::setw(4) << std::setfill('0') << addr << "] : " << errstr << std::endl;
}

void error_general_funcname(const std::string& funcname, const int line_num, const int addr, const std::string& errstr)
{
    std::cout << "[" << funcname << "] Error on line " << std::dec << line_num << " [0x" 
        << std::hex << std::setw(4) << std::setfill('0') << addr << "] : " << errstr << std::endl;
}


void msg_general(const int line_num, const std::string& msg)
{
    std::cout << "line [" << std::dec << line_num << "] : " << msg << std::endl;
}
void msg_general_funcname(const std::string& funcname, const int line_num, const std::string& msg)
{
    std::cout << "[" << funcname << "] line [" << std::dec << line_num << "] : " << msg << std::endl;
}
