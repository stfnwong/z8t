/*
 * COMMON
 * Stuff that gets used everywhere
 */

#ifndef __COMMON_HPP
#define __COMMON_HPP

#include <iostream>
#include <string>

#define LOG(o, s) do{ \
    o << "[" << __FUNCTION__ << ":" << __LINE__ << "] : " << s; \
    } while(0)


struct Z8TLogger 
{
    std::ostream& out_stream;

    public:
        Z8TLogger(std::ostream& stream = std::cout) : out_stream(stream) {}

        //operator std::ostream() 
        //{
        //    return this->out_stream;
        //}

        template <typename T> Z8TLogger& operator<<(const T& data)
        {
            this->out_stream << data;
            return *this;
        }
};



void error_general(const int line_num, const int addr, const std::string& errstr);
void error_general_funcname(const std::string& funcname, const int line_num, const int addr, const std::string& errstr);

// TODO; the logger above would actually be better than this...
void msg_general(const int line_num, const std::string& msg);
void msg_general_funcname(const std::string& funcname, const int line_num, const std::string& msg);

#endif /*__COMMON_HPP*/
