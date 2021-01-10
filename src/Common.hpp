/*
 * COMMON
 * Stuff that gets used everywhere
 */

#ifndef __COMMON_HPP
#define __COMMON_HPP

#include <string>


void error_general(const int line_num, const int addr, const std::string& errstr);
void error_general_funcname(const std::string& funcname, const int line_num, const int addr, const std::string& errstr);


#endif /*__COMMON_HPP*/
