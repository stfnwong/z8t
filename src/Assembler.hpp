/*
 * ASSEMBLER
 * Z80 assembler 
 *
 * Stefan Wong 2018
 */

#ifndef __ASSEMBLER_HPP
#define __ASSEMBLER_HPP

#include <string>
#include <unordered_map>
#include "Source.hpp"
#include "Program.hpp"


const std::unordered_map<uint32_t, int> instr_lookup = {
    // add 
    {(INSTR_ADD << 16) | (REG_A       << 8) | REG_A,           0x87},   // add a, a 
    {(INSTR_ADD << 16) | (REG_A       << 8) | REG_B,           0x80},   // add a, b 
    {(INSTR_ADD << 16) | (REG_A       << 8) | REG_C,           0x81},   // add a, c 
    {(INSTR_ADD << 16) | (REG_A       << 8) | REG_D,           0x82},   // add a, d 
    {(INSTR_ADD << 16) | (REG_A       << 8) | REG_E,           0x83},   // add a, e 
    {(INSTR_ADD << 16) | (REG_A       << 8) | REG_H,           0x84},   // add a, h 
    {(INSTR_ADD << 16) | (REG_A       << 8) | REG_L,           0x85},   // add a, l 
    {(INSTR_ADD << 16) | (REG_A       << 8) | REG_HL_IND,      0x86},   // add a, (hl) 
    {(INSTR_ADD << 16) | (REG_A       << 8) | SYM_LITERAL,     0xC6},   // add a, ** 
    {(INSTR_ADD << 16) | (REG_HL      << 8) | REG_BC,          0x09},   // add hl, bc 
    {(INSTR_ADD << 16) | (REG_HL      << 8) | REG_DE,          0x19},   // add hl, de 
    {(INSTR_ADD << 16) | (REG_HL      << 8) | REG_HL,          0x29},   // add hl, hl 
    {(INSTR_ADD << 16) | (REG_HL      << 8) | REG_SP,          0x39},   // add hl, sp 
    // adc
    {(INSTR_ADC << 16) | (REG_A       << 8) | REG_A,           0x8F},   // adc a, a 
    {(INSTR_ADC << 16) | (REG_A       << 8) | REG_B,           0x88},   // adc a, b 
    {(INSTR_ADC << 16) | (REG_A       << 8) | REG_C,           0x89},   // adc a, c 
    {(INSTR_ADC << 16) | (REG_A       << 8) | REG_D,           0x8A},   // adc a, d 
    {(INSTR_ADC << 16) | (REG_A       << 8) | REG_E,           0x8B},   // adc a, e 
    {(INSTR_ADC << 16) | (REG_A       << 8) | REG_H,           0x8C},   // adc a, h 
    {(INSTR_ADC << 16) | (REG_A       << 8) | REG_L,           0x8D},   // adc a, l 
    {(INSTR_ADC << 16) | (REG_A       << 8) | REG_HL_IND,      0x8E},   // adc a, (hl) 
    {(INSTR_ADC << 16) | (REG_A       << 8) | SYM_LITERAL,     0xCE},   // adc a, * 
    // and
    {(INSTR_AND << 16) | (REG_A       << 8),                   0xA7},   // and a
    {(INSTR_AND << 16) | (REG_B       << 8),                   0xA0},   // and b
    {(INSTR_AND << 16) | (REG_C       << 8),                   0xA1},   // and c
    {(INSTR_AND << 16) | (REG_D       << 8),                   0xA2},   // and d
    {(INSTR_AND << 16) | (REG_E       << 8),                   0xA3},   // and e
    {(INSTR_AND << 16) | (REG_H       << 8),                   0xA4},   // and h
    {(INSTR_AND << 16) | (REG_L       << 8),                   0xA5},   // and l
    {(INSTR_AND << 16) | (REG_HL_IND  << 8),                   0xA6},   // and (hl)
    {(INSTR_AND << 16) | (SYM_LITERAL << 8),                   0xE6},   // and *
    // inc 
    {(INSTR_INC << 16) | (REG_A       << 8),                   0x3C},   // inc a
    {(INSTR_INC << 16) | (REG_B       << 8),                   0x15},   // inc b
    {(INSTR_INC << 16) | (REG_C       << 8),                   0x0C},   // inc c
    {(INSTR_INC << 16) | (REG_D       << 8),                   0x14},   // inc d
    {(INSTR_INC << 16) | (REG_E       << 8),                   0x1C},   // inc e
    {(INSTR_INC << 16) | (REG_H       << 8),                   0x24},   // inc h
    {(INSTR_INC << 16) | (REG_L       << 8),                   0x2C},   // inc l
    {(INSTR_INC << 16) | (REG_BC      << 8),                   0x03},   // inc bc
    {(INSTR_INC << 16) | (REG_DE      << 8),                   0x13},   // inc de
    {(INSTR_INC << 16) | (REG_HL      << 8),                   0x23},   // inc hl
    {(INSTR_INC << 16) | (REG_SP      << 8),                   0x33},   // inc sp
    {(INSTR_INC << 16) | (REG_HL_IND  << 8),                   0x34},   // inc (hl)
    // ld (a)
    {(INSTR_LD << 16)  | (REG_A       << 8) | REG_A,           0x7F},   // ld a, a
    {(INSTR_LD << 16)  | (REG_A       << 8) | REG_B,           0x78},   // ld a, b
    {(INSTR_LD << 16)  | (REG_A       << 8) | REG_C,           0x79},   // ld a, c
    {(INSTR_LD << 16)  | (REG_A       << 8) | REG_D,           0x7A},   // ld a, d
    {(INSTR_LD << 16)  | (REG_A       << 8) | REG_E,           0x7B},   // ld a, e
    {(INSTR_LD << 16)  | (REG_A       << 8) | REG_H,           0x7C},   // ld a, h
    {(INSTR_LD << 16)  | (REG_A       << 8) | REG_L,           0x7D},   // ld a, l
    {(INSTR_LD << 16)  | (REG_A       << 8) | REG_HL_IND,      0x7E},   // ld a, (hl)
    {(INSTR_LD << 16)  | (REG_A       << 8) | REG_BC_IND,      0x0A},   // ld a, (bc)
    {(INSTR_LD << 16)  | (REG_A       << 8) | REG_DE_IND,      0x1A},   // ld a, (de)
    {(INSTR_LD << 16)  | (REG_A       << 8) | SYM_LITERAL,     0x3E},   // ld a, *
    {(INSTR_LD << 16)  | (REG_A       << 8) | SYM_LITERAL_IND, 0x3A},   // ld a, (**)
    // ld (b)
    {(INSTR_LD << 16)  | (REG_B       << 8) | REG_A,           0x06},   // ld b, a
    {(INSTR_LD << 16)  | (REG_B       << 8) | REG_B,           0x40},   // ld b, b
    {(INSTR_LD << 16)  | (REG_B       << 8) | REG_C,           0x41},   // ld b, c
    {(INSTR_LD << 16)  | (REG_B       << 8) | REG_D,           0x42},   // ld b, d
    {(INSTR_LD << 16)  | (REG_B       << 8) | REG_E,           0x43},   // ld b, e
    {(INSTR_LD << 16)  | (REG_B       << 8) | REG_H,           0x44},   // ld b, h
    {(INSTR_LD << 16)  | (REG_B       << 8) | REG_L,           0x45},   // ld b, l
    {(INSTR_LD << 16)  | (REG_B       << 8) | REG_HL_IND,      0x46},   // ld b, (hl)
    {(INSTR_LD << 16)  | (REG_B       << 8) | SYM_LITERAL,     0x06},   // ld b, *
    // ld (c)
    {(INSTR_LD << 16)  | (REG_C       << 8) | REG_A,           0x4F},   // ld c, a
    {(INSTR_LD << 16)  | (REG_C       << 8) | REG_B,           0x48},   // ld c, b
    {(INSTR_LD << 16)  | (REG_C       << 8) | REG_C,           0x49},   // ld c, c
    {(INSTR_LD << 16)  | (REG_C       << 8) | REG_D,           0x4A},   // ld c, d
    {(INSTR_LD << 16)  | (REG_C       << 8) | REG_E,           0x4B},   // ld c, e
    {(INSTR_LD << 16)  | (REG_C       << 8) | REG_H,           0x4C},   // ld c, h
    {(INSTR_LD << 16)  | (REG_C       << 8) | REG_L,           0x4D},   // ld c, l
    {(INSTR_LD << 16)  | (REG_C       << 8) | REG_HL_IND,      0x4E},   // ld c, (hl)
    {(INSTR_LD << 16)  | (REG_C       << 8) | SYM_LITERAL,     0x0E},   // ld c, *
    // ld (d)
    {(INSTR_LD << 16)  | (REG_D       << 8) | REG_A,           0x57},   // ld d, a
    {(INSTR_LD << 16)  | (REG_D       << 8) | REG_B,           0x50},   // ld d, b
    {(INSTR_LD << 16)  | (REG_D       << 8) | REG_C,           0x51},   // ld d, c
    {(INSTR_LD << 16)  | (REG_D       << 8) | REG_D,           0x52},   // ld d, d
    {(INSTR_LD << 16)  | (REG_D       << 8) | REG_E,           0x53},   // ld d, e
    {(INSTR_LD << 16)  | (REG_D       << 8) | REG_H,           0x54},   // ld d, h
    {(INSTR_LD << 16)  | (REG_D       << 8) | REG_L,           0x55},   // ld d, l
    {(INSTR_LD << 16)  | (REG_D       << 8) | REG_HL_IND,      0x56},   // ld d, (hl)
    {(INSTR_LD << 16)  | (REG_D       << 8) | SYM_LITERAL,     0x06},   // ld d, *
    // ld (e)
    {(INSTR_LD << 16)  | (REG_E       << 8) | REG_A,           0x5F},   // ld e, a
    {(INSTR_LD << 16)  | (REG_E       << 8) | REG_B,           0x58},   // ld e, b
    {(INSTR_LD << 16)  | (REG_E       << 8) | REG_C,           0x59},   // ld e, c
    {(INSTR_LD << 16)  | (REG_E       << 8) | REG_D,           0x5A},   // ld e, d
    {(INSTR_LD << 16)  | (REG_E       << 8) | REG_E,           0x5B},   // ld e, e
    {(INSTR_LD << 16)  | (REG_E       << 8) | REG_H,           0x5C},   // ld e, h
    {(INSTR_LD << 16)  | (REG_E       << 8) | REG_L,           0x5D},   // ld e, l
    {(INSTR_LD << 16)  | (REG_E       << 8) | REG_HL_IND,      0x5E},   // ld e, (hl)
    {(INSTR_LD << 16)  | (REG_E       << 8) | SYM_LITERAL,     0x1E},   // ld e, *
    // ld (h)
    {(INSTR_LD << 16)  | (REG_H       << 8) | REG_A,           0x67},   // ld h, a
    {(INSTR_LD << 16)  | (REG_H       << 8) | REG_B,           0x60},   // ld h, b
    {(INSTR_LD << 16)  | (REG_H       << 8) | REG_C,           0x61},   // ld h, c
    {(INSTR_LD << 16)  | (REG_H       << 8) | REG_D,           0x62},   // ld h, d
    {(INSTR_LD << 16)  | (REG_H       << 8) | REG_E,           0x63},   // ld h, e
    {(INSTR_LD << 16)  | (REG_H       << 8) | REG_H,           0x64},   // ld h, h
    {(INSTR_LD << 16)  | (REG_H       << 8) | REG_L,           0x65},   // ld h, l
    {(INSTR_LD << 16)  | (REG_H       << 8) | REG_HL_IND,      0x66},   // ld h, (hl)
    {(INSTR_LD << 16)  | (REG_H       << 8) | SYM_LITERAL,     0x26},   // ld h, *
    // ld (l)
    {(INSTR_LD << 16)  | (REG_L       << 8) | REG_A,           0x6F},   // ld l, a
    {(INSTR_LD << 16)  | (REG_L       << 8) | REG_B,           0x68},   // ld l, b
    {(INSTR_LD << 16)  | (REG_L       << 8) | REG_C,           0x69},   // ld l, c
    {(INSTR_LD << 16)  | (REG_L       << 8) | REG_D,           0x6A},   // ld l, d
    {(INSTR_LD << 16)  | (REG_L       << 8) | REG_E,           0x6B},   // ld l, e
    {(INSTR_LD << 16)  | (REG_L       << 8) | REG_H,           0x6C},   // ld l, h
    {(INSTR_LD << 16)  | (REG_L       << 8) | REG_L,           0x6D},   // ld l, l
    {(INSTR_LD << 16)  | (REG_L       << 8) | REG_HL_IND,      0x6E},   // ld l, (hl)
    {(INSTR_LD << 16)  | (REG_L       << 8) | SYM_LITERAL,     0x2E},   // ld l, *
    // ld (indrections)
    {(INSTR_LD << 16)  | (REG_HL_IND  << 8) | REG_A,           0x77},   // ld (hl), a
    {(INSTR_LD << 16)  | (REG_HL_IND  << 8) | REG_B,           0x70},   // ld (hl), b
    {(INSTR_LD << 16)  | (REG_HL_IND  << 8) | REG_C,           0x71},   // ld (hl), c
    {(INSTR_LD << 16)  | (REG_HL_IND  << 8) | REG_D,           0x72},   // ld (hl), d
    {(INSTR_LD << 16)  | (REG_HL_IND  << 8) | REG_H,           0x73},   // ld (hl), e
    {(INSTR_LD << 16)  | (REG_HL_IND  << 8) | REG_H,           0x74},   // ld (hl), h
    {(INSTR_LD << 16)  | (REG_HL_IND  << 8) | REG_L,           0x75},   // ld (hl), l
    {(INSTR_LD << 16)  | (REG_HL_IND  << 8) | REG_HL_IND,      0x76},   // ld (hl), (hl)
    {(INSTR_LD << 16)  | (REG_HL_IND  << 8) | SYM_LITERAL,     0x36},   // ld (hl), *
    {(INSTR_LD << 16)  | (REG_BC_IND  << 8) | REG_A,           0x02},   // ld (bc), a
    {(INSTR_LD << 16)  | (REG_DE_IND  << 8) | REG_A,           0x12},   // ld (de), a

    {(INSTR_LD << 16)  | (REG_BC      << 8) | SYM_LITERAL,     0x01},   // ld bc, **
    {(INSTR_LD << 16)  | (REG_DE      << 8) | SYM_LITERAL,     0x11},   // ld de, **
    {(INSTR_LD << 16)  | (REG_HL      << 8) | SYM_LITERAL,     0x21},   // ld hl, **
    {(INSTR_LD << 16)  | (REG_SP      << 8) | SYM_LITERAL,     0x31},   // ld sp, **
    // ld (**)
    {(INSTR_LD << 16)  | (SYM_LITERAL_IND << 8) | REG_HL,      0x22},   // ld (**), hl

    // or 
    {(INSTR_OR << 16)  | (REG_A       << 8),                   0xB7},   // or a
    {(INSTR_OR << 16)  | (REG_B       << 8),                   0xB0},   // or b
    {(INSTR_OR << 16)  | (REG_C       << 8),                   0xB1},   // or c
    {(INSTR_OR << 16)  | (REG_D       << 8),                   0xB2},   // or d
    {(INSTR_OR << 16)  | (REG_E       << 8),                   0xB3},   // or e
    {(INSTR_OR << 16)  | (REG_H       << 8),                   0xB4},   // or h
    {(INSTR_OR << 16)  | (REG_L       << 8),                   0xB5},   // or l
    {(INSTR_OR << 16)  | (REG_HL_IND  << 8),                   0xB6},   // or (hl)
    {(INSTR_OR << 16)  | (SYM_LITERAL << 8),                   0xF6},   // or *
    // ret
    {(INSTR_RET << 16)  | (COND_NZ    << 8),                   0xC0},   // ret nz
    {(INSTR_RET << 16)  | (COND_NC    << 8),                   0xD0},   // ret nc
    {(INSTR_RET << 16)  | (COND_PO    << 8),                   0xE0},   // ret po
    {(INSTR_RET << 16)  | (COND_P     << 8),                   0xF0},   // ret p
    {(INSTR_RET << 16)  | (COND_Z     << 8),                   0xC8},   // ret z
    {(INSTR_RET << 16)  | (COND_C     << 8),                   0xD8},   // ret c
    {(INSTR_RET << 16)  | (COND_PE    << 8),                   0xE8},   // ret pe
    {(INSTR_RET << 16)  | (COND_M     << 8),                   0xF8},   // ret m
    {(INSTR_RET << 16)  ,                                      0xC9},   // ret 
    // sbc
    {(INSTR_SBC << 16) | (REG_A       << 8) | REG_A,           0x9F},   // sbc a, a 
    {(INSTR_SBC << 16) | (REG_A       << 8) | REG_B,           0x98},   // sbc a, b 
    {(INSTR_SBC << 16) | (REG_A       << 8) | REG_C,           0x99},   // sbc a, c 
    {(INSTR_SBC << 16) | (REG_A       << 8) | REG_D,           0x9A},   // sbc a, d 
    {(INSTR_SBC << 16) | (REG_A       << 8) | REG_E,           0x9B},   // sbc a, e 
    {(INSTR_SBC << 16) | (REG_A       << 8) | REG_H,           0x9C},   // sbc a, h 
    {(INSTR_SBC << 16) | (REG_A       << 8) | REG_L,           0x9D},   // sbc a, l 
    {(INSTR_SBC << 16) | (REG_A       << 8) | REG_HL_IND,      0x9E},   // sbc a, (hl) 
    {(INSTR_SBC << 16) | (REG_A       << 8) | SYM_LITERAL,     0xDE},   // sbc a, * 
    // sub
    {(INSTR_SUB << 16) | (REG_A       << 8),                   0x97},   // sub a
    {(INSTR_SUB << 16) | (REG_B       << 8),                   0x90},   // sub b
    {(INSTR_SUB << 16) | (REG_C       << 8),                   0x91},   // sub c
    {(INSTR_SUB << 16) | (REG_D       << 8),                   0x92},   // sub d
    {(INSTR_SUB << 16) | (REG_E       << 8),                   0x93},   // sub e
    {(INSTR_SUB << 16) | (REG_H       << 8),                   0x94},   // sub h
    {(INSTR_SUB << 16) | (REG_L       << 8),                   0x95},   // sub l
    {(INSTR_SUB << 16) | (REG_HL_IND  << 8),                   0x96},   // sub (hl)
    {(INSTR_SUB << 16) | (SYM_LITERAL << 8),                   0xD6},   // sub *
    // xor
    {(INSTR_XOR << 16) | (REG_A       << 8),                   0xAF},   // xor a
    {(INSTR_XOR << 16) | (REG_B       << 8),                   0xA8},   // xor b
    {(INSTR_XOR << 16) | (REG_C       << 8),                   0xA9},   // xor c
    {(INSTR_XOR << 16) | (REG_D       << 8),                   0xAA},   // xor d
    {(INSTR_XOR << 16) | (REG_E       << 8),                   0xAB},   // xor e
    {(INSTR_XOR << 16) | (REG_H       << 8),                   0xAC},   // xor h
    {(INSTR_XOR << 16) | (REG_L       << 8),                   0xAD},   // xor l
    {(INSTR_XOR << 16) | (REG_HL_IND  << 8),                   0xAE},   // xor (hl)
    {(INSTR_XOR << 16) | (SYM_LITERAL << 8),                   0xEE}    // xor *
};


class Assembler
{
    private:
        bool verbose;
        SourceInfo source;
        Program program;

    private:
        // assembly routines
        Instr asm_add(const TextLine& line) const;
        Instr asm_and(const TextLine& line) const;
        Instr asm_dec(const TextLine& line) const;
        Instr asm_ld(const TextLine& line) const;

    public:
        Assembler();

        void init(void);
        void assemble(void);

        void loadSource(const SourceInfo& s);
        void setVerbose(const bool v);
        bool getVerbose(void) const;

        // getters, setters 
        Program getProgram(void) const;
};

#endif /*__ASSEMBLER_HPP*/
