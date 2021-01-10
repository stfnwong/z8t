/*
 * COMMON
 * Stuff that gets used everywhere
 */

#include "Common.hpp"

#include <iostream>
#include <iomanip>


void error_general(const int line_num, const int addr, const std::string& errstr) 
{
    std::cout << "Error on line " << line_num << " [Address 0x" 
        << std::hex << std::setw(4) << addr << "] : " << errstr << std::endl;
}

void error_general_funcname(const std::string& funcname, const int line_num, const int addr, const std::string& errstr)
{
    std::cout << "[" << funcname << "] Error on line " << line_num << " [Address 0x" 
        << std::hex << std::setw(4) << addr << "] : " << errstr << std::endl;
}

