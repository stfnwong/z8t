/*
 * LOOKUP
 * Various lookup tables
 */

#ifndef __LOOKUP_HPP
#define __LOOKUP_HPP

#include <string>
#include <unordered_map>


template <typename T> struct Lookup
{
    std::unordered_map<std::string, T> item_to_idx;
    std::unordered_map<T, std::string> idx_to_item;

    public:
        Lookup();
        std::string getName(const int idx) const;
        T getIdx(const std::string& name) const;
        unsigned int size(void) const;
};


template <typename T> Lookup<T>::Lookup(const T[]& inputs)
{
    for(const T& elem : inputs)
    {
        this->item_to_idx[elem.val] = elem.repr;
        this->item_to_idx[elem.repr] = elem.val;
    }
}


#endif /*__LOOKUP_HPP*/
