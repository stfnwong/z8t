/*
 * REGISTER
 * Stuff to map registers 
 */

#include <iostream>
#include <iomanip>
#include <sstream>
#include "Register.hpp"


Condition::Condition() : idx(0), repr("\0") {} 

Condition::Condition(const int v, const std::string& r) : idx(v), repr(r) {} 

void Condition::init(void)
{
    this->idx = 0;
    this->repr.clear();
}

bool Condition::operator==(const Condition& that) const
{
    if(this->idx != that.idx)
        return false;
    if(this->repr != that.repr)
        return false;

    return true;
}

bool Condition::operator!=(const Condition& that) const
{
    return !(*this == that);
}


/* 
 * REGISTER
 */
Register::Register() : val(0), repr("") {} 

Register::Register(int i, const std::string& n) : val(i), repr(n) {} 

/*
 * Register::==
 */
bool Register::operator==(const Register& that) const
{
    if(this->val != that.val)
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
        this->val = that.val;
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

    oss << this->repr << "[" << std::dec << this->val << "]";

    return oss.str();
}


/* 
 * REGISTERMAP
 */
RegisterMap::RegisterMap()
{
    for (const Register& reg : Z80_REGISTERS)
    {
        this->reg_idx_to_reg_name[reg.val] = reg.repr;
        this->reg_name_to_reg_idx[reg.repr] = reg.val;
    }
}


std::string RegisterMap::getName(const int idx) const
{
    auto name = this->reg_idx_to_reg_name.find(idx);
    if(name != this->reg_idx_to_reg_name.end())
        return name->second;

    return this->invalid_name;
}

int RegisterMap::getIdx(const std::string& name) const
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

// REGISTER TABLE
// TODO : refactor all tables to have a common ancestor
RegisterTable::RegisterTable()
{

}

void RegisterTable::add(const Register& r)
{

}


/// TODO: probably refactor this 
/*
 * CondMap
 */
CondMap::CondMap()
{
    for (const Condition& cond : Z80_CONDITIONS)
    {
        this->cond_idx_to_cond_name[cond.idx] = cond.repr;
        this->cond_name_to_cond_idx[cond.repr] = cond.idx;
    }
}
