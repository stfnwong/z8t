/*
 * CPU
 * Components in the Z80 CPU
 */

#ifndef __CPU_HPP
#define __CPU_HPP

#include <cstdint>


struct CPUState
{
    // 16-bit registers
    uint16_t pc;
    uint16_t sp;
    uint16_t ix;
    uint16_t iy;

    // TODO: note that acc, flags, and the 16-bit pairs also have a set
    // of shadow registers. Implement these later (eg: bc_prime)
    // accumulator register
    uint8_t acc;
    // flags register
    uint8_t flags;

    // 8-bit register pairs
    uint16_t bc;
    uint16_t de;
    uint16_t hl;
    uint16_t wz;        // internal transfer registers

    // 8-bit registers

    public:
        CPUState();

        bool operator==(const CPUState& that) const;
        bool operator!=(const CPUState& that) const;

        // helper functions to read only upper or lower 8-bits of BC, DE, or HL
        uint8_t  read_b(void) const;        // b is upper
        uint8_t  read_c(void) const;        // c is lower
        uint8_t  read_d(void) const;        // d is upper
        uint8_t  read_e(void) const;        // e is lower 
        uint8_t  read_h(void) const;        // h is upper
        uint8_t  read_l(void) const;        // l is lower
        uint8_t  read_w(void) const;        // w is upper
        uint8_t  read_z(void) const;        // z is lower

        // read 16-bit words
        uint16_t read_bc(void) const;
        uint16_t read_de(void) const;
        uint16_t read_hl(void) const;
        uint16_t read_wz(void) const;

        // write only upper or lower 8-bits of BC, DE, or HL
        void write_b(uint8_t v);   // b is upper
        void write_c(uint8_t v);   // c is lower
        void write_d(uint8_t v);   // d is upper
        void write_e(uint8_t v);   // e is lower 
        void write_h(uint8_t v);   // h is upper
        void write_l(uint8_t v);   // l is lower
        void write_w(uint8_t v);   // w is upper
        void write_z(uint8_t v);   // z is lower

        // write 16-bit words 
        void write_bc(uint16_t v);
        void write_de(uint16_t v);
        void write_hl(uint16_t v);
        void write_wz(uint16_t v);
};



#endif /*__CPU_HPP*/