/*
 * UTIL
 * Things that don't fit anywhere else
 */

#ifndef __UTIL_HPP
#define __UTIL_HPP

#include <cmath>

#define COMP_EPSILON 1e-5

inline bool equal(float a, float b)
{
    return (std::fabs(a - b) < COMP_EPSILON) ? true : false;
}


#endif /*__UTIL_HPP*/
