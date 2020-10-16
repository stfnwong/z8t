/*
 * REGISTER
 * Stuff to map registers 
 */

#ifndef __REGISTER_HPP
#define __REGISTER_HPP

#include <unordered_map>


// Register names 
typedef enum Z80_REG
{
    REG_A, 
    REG_B, 
    REG_C, 
    REG_D, 
    REG_E, 
    REG_H, 
    REG_L, 
    REG_IXH, 
    REG_IXL, 
    REG_IYH, 
    REG_IYL, 
    REG_HL, 
    REG_BC
} Z80_REG;




/*
 * Register
 * Structure representation of a named register
 * in the Z80 processor. 
 */
struct Register
{
    int idx;
    std::string repr;

    public:
        Register();
        Register(int i, const std::string& n);
        //Register(const Register& that) = default;

        bool operator==(const Register& that) const;
        bool operator!=(const Register& that) const;
        Register& operator=(const Register& that) ;
        void init(void);
        std::string toString(void) const;
};

/*
 * RegisterMap
 * Wraps two unordered maps that related register strings to register positions
 */
struct RegisterMap
{
    std::string invalid_name = "$?";
    int invalid_idx = -1;
    std::unordered_map<std::string, int> reg_name_to_reg_idx;
    std::unordered_map<int, std::string> reg_idx_to_reg_name;

    public:
        RegisterMap();
        std::string getName(const int idx) ;
        int getIdx(const std::string& name) ;
        unsigned int size(void) const;
};

const Register Z80_REGISTERS[] = {
    Register(REG_A, "a"),
    Register(REG_B, "b"),
    Register(REG_C, "c"),
};


#endif /*__REGISTER_HPP*/
