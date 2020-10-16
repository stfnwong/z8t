/*
 * REGISTER
 * Stuff to map registers 
 */

#include <iostream>
#include <iomanip>
#include <sstream>
#include "Register.hpp"

/* 
 * REGISTER
 */
Register::Register() : idx(0), repr("") {} 

Register::Register(int i, const std::string& n) : idx(i), repr(n) {} 

/*
 * Register::==
 */
bool Register::operator==(const Register& that) const
{
    if(this->idx != that.idx)
        return false;
    if(this->repr != that.repr)
        return false;
    return true;
}

/*
 * Register::!=
 */
bool Register::operator!=(const Register& that) const
{
    return !(*this == that);
}

/*
 * Register::=
 */
Register& Register::operator=(const Register& that) 
{
    if(this != &that)
    {
        this->idx = that.idx;
        this->repr = that.repr;
    }

    return *this;
}

/*
 * Register::toString()
 */
std::string Register::toString(void) const
{
    std::ostringstream oss;

    oss << this->repr << "[" << std::dec << this->idx << "]";

    return oss.str();
}


/* 
 * REGISTERMAP
 */
RegisterMap::RegisterMap()
{
    for (const Register& reg : Z80_REGISTERS)
    {
        this->reg_idx_to_reg_name[reg.idx] = reg.repr;
        this->reg_name_to_reg_idx[reg.repr] = reg.idx;
    }
}


std::string RegisterMap::getName(const int idx) 
{
    auto name = this->reg_idx_to_reg_name.find(idx);
    if(name != this->reg_idx_to_reg_name.end())
        return name->second;

    return this->invalid_name;
}

int RegisterMap::getIdx(const std::string& name) 
{
    auto idx = this->reg_name_to_reg_idx.find(name);
    if(idx != this->reg_name_to_reg_idx.end())
        return idx->second;

    return this->invalid_idx;
}

unsigned int RegisterMap::size(void) const
{
    return this->reg_idx_to_reg_name.size();
}
