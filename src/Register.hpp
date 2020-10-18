/*
 * REGISTER
 * Stuff to map registers 
 */

#ifndef __REGISTER_HPP
#define __REGISTER_HPP

#include <unordered_map>

/*
 * Register
 * Structure representation of a named register
 * in the Z80 processor. 
 */
struct Register
{
    int val;
    std::string repr;

    public:
        Register();
        Register(int i, const std::string& n);
        Register(const Register& that) = default;

        bool operator==(const Register& that) const;
        bool operator!=(const Register& that) const;
        Register& operator=(const Register& that) ;
        void init(void);
        std::string toString(void) const;
};


// Register names 
typedef enum Z80_REG
{
    REG_NULL,
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


// Register objects
const Register Z80_REGISTERS[] = 
{
    Register(REG_A, "a"),
    Register(REG_B, "b"),
    Register(REG_C, "c"),
    Register(REG_D, "d"),
    Register(REG_E, "e"),
    Register(REG_H, "h"),
    Register(REG_L, "l"),
    Register(REG_HL, "hl"),
    Register(REG_BC, "bc"),
};


// TODO: genericize into AsmPrimitive later...
struct Condition
{
    int idx;
    std::string repr;

    public:
        Condition();
        Condition(const int v, const std::string& r);

        void init(void);
        bool operator==(const Condition& that) const;
        bool operator!=(const Condition& that) const;
};

// Condition names
typedef enum
{
    COND_C,
    COND_NC, 
    COND_Z,
    COND_NZ,
    COND_M,
    COND_P,
    COND_PE,
} Z80_COND;


const Condition Z80_CONDITIONS[] = 
{
    Condition(COND_C,  "C"),
    Condition(COND_NC, "NC"),
    Condition(COND_Z,  "Z"),
    Condition(COND_NZ, "NZ"),
    Condition(COND_M,  "M"),
    Condition(COND_P,  "P"),
    Condition(COND_PE, "PE"),
};


// TODO : the generic version is what? BidirMap?

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

        std::string getName(const int idx) const;
        int getIdx(const std::string& name) const;
        unsigned int size(void) const;
};

// TODO : another similar table structure? 
class RegisterTable
{
    std::unordered_map<std::string, Register> reg_map;

    public:
        RegisterTable();
        void add(const Register& r);
        Register get(const std::string& reg_str) const;
};


// TODO : this can probably be made more generic...
struct CondMap
{
    std::string invalid_name = "X";
    int invalid_idx = -1;

    std::unordered_map<std::string, int> cond_name_to_cond_idx;
    std::unordered_map<int, std::string> cond_idx_to_cond_name;

    public:
        CondMap();

        std::string getName(const int idx) const;
        int getIdx(const std::string& name) const;
        unsigned int size(void) const;
};


#endif /*__REGISTER_HPP*/
