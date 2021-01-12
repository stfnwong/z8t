/*
 * EVAL
 * Stuff common for expression evaluation
 */

#include <iostream>
#include <iomanip>
#include <sstream>

#include "Eval.hpp"


/*
 * ======== EVAL RESULT ======== //
 */
EvalResult::EvalResult() : val(0), ok(false) {} 
EvalResult::EvalResult(const int v) : val(v), ok(true) {} 
EvalResult::EvalResult(const int v, const bool o) : val(v), ok(o) {}

void EvalResult::init(void)
{
    this->val = 0;
    this->ok = false;
}

EvalResult& EvalResult::operator=(const EvalResult& that) 
{
    if(this != &that)
    {
        this->val = that.val;
        this->ok = that.ok;
    }

    return *this;
}


bool EvalResult::operator==(const EvalResult& that) const
{
    if(this->val != that.val)
        return false;
    if(this->ok != that.ok)
        return false;

    return true;
}

bool EvalResult::operator!=(const EvalResult& that) const
{
    return !(*this == that);
}

std::string EvalResult::toString(void) const
{
    std::ostringstream oss;

    oss << "EvalResult: [" << std::dec << this->val << "] ";
    if(this->ok)
        oss << " OK ";
    
    return oss.str();
}
