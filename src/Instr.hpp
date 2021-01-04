/*
 * INSTR
 * Instruction decode logic
 */

#ifndef __INSTR_HPP
#define __INSTR_HPP

#include <cstdint>
#include <unordered_map>

//typedef enum {
//    INSTR_ADD, 
//    INSTR_ADC,
//    INSTR_AND, 
//    INSTR_CALL,
//    INSTR_CCF,
//    INSTR_CP,
//    INSTR_CPL,
//    INSTR_DAA,
//    INSTR_DEC, 
//    INSTR_DI,
//    INSTR_DJNZ,
//    INSTR_EX,
//    INSTR_JP,
//    INSTR_JR,
//    INSTR_LD, 
//    INSTR_INC, 
//    INSTR_OR,
//    INSTR_OUT,
//    INSTR_POP, 
//    INSTR_PUSH,
//    INSTR_RRA,
//    INSTR_RRCA,
//    INSTR_RET,
//    INSTR_RST,
//    INSTR_SBC,
//    INSTR_SUB,
//    INSTR_XOR
//} Z80_INSTR;


// This map just returns the size of each instruction 
static const std::unordered_map<uint8_t, uint8_t> instr_size_map {
    // add 
    {0x87, 1},   // add a, a 
    {0x80, 1},   // add a, b 
    {0x81, 1},   // add a, c 
    {0x82, 1},   // add a, d 
    {0x83, 1},   // add a, e 
    {0x84, 1},   // add a, h 
    {0x85, 1},   // add a, l 
    {0x86, 1},   // add a, (hl) 
    {0xC6, 2},   // add a, ** 
    {0x09, 1},   // add hl, bc 
    {0x19, 1},   // add hl, de 
    {0x29, 1},   // add hl, hl 
    {0x39, 1},   // add hl, sp 
    // adc
    {0x8F, 1},   // adc a, a 
    {0x88, 1},   // adc a, b 
    {0x89, 1},   // adc a, c 
    {0x8A, 1},   // adc a, d 
    {0x8B, 1},   // adc a, e 
    {0x8C, 1},   // adc a, h 
    {0x8D, 1},   // adc a, l 
    {0x8E, 1},   // adc a, (hl) 
    {0xCE, 2},   // adc a, * 
    // and
    {0xA7, 1},   // and a
    {0xA0, 1},   // and b
    {0xA1, 1},   // and c
    {0xA2, 1},   // and d
    {0xA3, 1},   // and e
    {0xA4, 1},   // and h
    {0xA5, 1},   // and l
    {0xA6, 1},   // and (hl)
    {0xE6, 2},   // and *
    // cp 
    {0xBF, 1},   // cp a
    {0xB8, 1},   // cp b
    {0xB9, 1},   // cp c
    {0xBA, 1},   // cp d
    {0xBB, 1},   // cp e
    {0xBC, 1},   // cp h
    {0xBD, 1},   // cp l
    {0xBE, 1},   // cp (hl)
    {0xFE, 2},   // cp *
    // inc 
    {0x3C, 1},   // inc a
    {0x15, 1},   // inc b
    {0x0C, 1},   // inc c
    {0x14, 1},   // inc d
    {0x1C, 1},   // inc e
    {0x24, 1},   // inc h
    {0x2C, 1},   // inc l
    {0x03, 1},   // inc bc
    {0x13, 1},   // inc de
    {0x23, 1},   // inc hl
    {0x33, 1},   // inc sp
    {0x34, 1},   // inc (hl)
    // jr 
    {0x18, 2},   // jr *
    {0x28, 2},   // jr z, *
    {0x38, 2},   // jr c, *
    {0x20, 2},   // jr nz, *
    {0x30, 2},   // jr nc, *
    // jp 
    {0xC3, 3},   // jp nz, **
    {0xD3, 3},   // jp nc, **
    {0xE3, 3},   // jp po, **
    {0xF3, 3},   // jp p, **
    {0xC4, 3},   // jp **
    {0xE9, 3},   // jp (hl)
    {0xCA, 3},   // jp z, **
    {0xDA, 3},   // jp c, **
    {0xEA, 3},   // jp pe, **
    {0xFA, 3},   // jp m, **
    // ld (a)
    {0x7F, 1},   // ld a, a
    {0x78, 1},   // ld a, b
    {0x79, 1},   // ld a, c
    {0x7A, 1},   // ld a, d
    {0x7B, 1},   // ld a, e
    {0x7C, 1},   // ld a, h
    {0x7D, 1},   // ld a, l
    {0x7E, 1},   // ld a, (hl)
    {0x0A, 1},   // ld a, (bc)
    {0x1A, 1},   // ld a, (de)
    {0x3E, 2},   // ld a, *
    {0x3A, 2},   // ld a, (**)
    // ld (b)
    {0x06, 1},   // ld b, a
    {0x40, 1},   // ld b, b
    {0x41, 1},   // ld b, c
    {0x42, 1},   // ld b, d
    {0x43, 1},   // ld b, e
    {0x44, 1},   // ld b, h
    {0x45, 1},   // ld b, l
    {0x46, 1},   // ld b, (hl)
    {0x06, 2},   // ld b, *
    // ld (c)
    {0x4F, 1},   // ld c, a
    {0x48, 1},   // ld c, b
    {0x49, 1},   // ld c, c
    {0x4A, 1},   // ld c, d
    {0x4B, 1},   // ld c, e
    {0x4C, 1},   // ld c, h
    {0x4D, 1},   // ld c, l
    {0x4E, 1},   // ld c, (hl)
    {0x0E, 2},   // ld c, *
    // ld (d)
    {0x57, 1},   // ld d, a
    {0x50, 1},   // ld d, b
    {0x51, 1},   // ld d, c
    {0x52, 1},   // ld d, d
    {0x53, 1},   // ld d, e
    {0x54, 1},   // ld d, h
    {0x55, 1},   // ld d, l
    {0x56, 1},   // ld d, (hl)
    {0x06, 2},   // ld d, *
    // ld (e)
    {0x5F, 1},   // ld e, a
    {0x58, 1},   // ld e, b
    {0x59, 1},   // ld e, c
    {0x5A, 1},   // ld e, d
    {0x5B, 1},   // ld e, e
    {0x5C, 1},   // ld e, h
    {0x5D, 1},   // ld e, l
    {0x5E, 1},   // ld e, (hl)
    {0x1E, 2},   // ld e, *
    // ld (h)
    {0x67, 1},   // ld h, a
    {0x60, 1},   // ld h, b
    {0x61, 1},   // ld h, c
    {0x62, 1},   // ld h, d
    {0x63, 1},   // ld h, e
    {0x64, 1},   // ld h, h
    {0x65, 1},   // ld h, l
    {0x66, 1},   // ld h, (hl)
    {0x26, 2},   // ld h, *
    // ld (l)
    {0x6F, 1},   // ld l, a
    {0x68, 1},   // ld l, b
    {0x69, 1},   // ld l, c
    {0x6A, 1},   // ld l, d
    {0x6B, 1},   // ld l, e
    {0x6C, 1},   // ld l, h
    {0x6D, 1},   // ld l, l
    {0x6E, 1},   // ld l, (hl)
    {0x2E, 2},   // ld l, *
    // ld (indrections)
    {0x77, 1},   // ld (hl), a
    {0x70, 1},   // ld (hl), b
    {0x71, 1},   // ld (hl), c
    {0x72, 1},   // ld (hl), d
    {0x73, 1},   // ld (hl), e
    {0x74, 1},   // ld (hl), h
    {0x75, 1},   // ld (hl), l
    {0x76, 1},   // ld (hl), (hl)
    {0x36, 2},   // ld (hl), *
    {0x02, 1},   // ld (bc), a
    {0x12, 1},   // ld (de), a
    // ld (**)
    {0x22, 3},   // ld (**), hl
    {0x01, 3},   // ld bc, **
    {0x11, 3},   // ld de, **
    {0x21, 3},   // ld hl, **
    {0x31, 3},   // ld sp, **

    // or 
    {0xB7, 1},   // or a
    {0xB0, 1},   // or b
    {0xB1, 1},   // or c
    {0xB2, 1},   // or d
    {0xB3, 1},   // or e
    {0xB4, 1},   // or h
    {0xB5, 1},   // or l
    {0xB6, 1},   // or (hl)
    {0xF6, 2},   // or *
    // ret
    {0xC0, 1},   // ret nz
    {0xD0, 1},   // ret nc
    {0xE0, 1},   // ret po
    {0xF0, 1},   // ret p
    {0xC8, 1},   // ret z
    {0xD8, 1},   // ret c
    {0xE8, 1},   // ret pe
    {0xF8, 1},   // ret m
    {0xC9, 1},   // ret 
    // sbc
    {0x9F, 1},   // sbc a, a 
    {0x98, 1},   // sbc a, b 
    {0x99, 1},   // sbc a, c 
    {0x9A, 1},   // sbc a, d 
    {0x9B, 1},   // sbc a, e 
    {0x9C, 1},   // sbc a, h 
    {0x9D, 1},   // sbc a, l 
    {0x9E, 1},   // sbc a, (hl) 
    {0xDE, 2},   // sbc a, * 
    // sub
    {0x97, 1},   // sub a
    {0x90, 1},   // sub b
    {0x91, 1},   // sub c
    {0x92, 1},   // sub d
    {0x93, 1},   // sub e
    {0x94, 1},   // sub h
    {0x95, 1},   // sub l
    {0x96, 1},   // sub (hl)
    {0xD6, 2},   // sub *
    // xor
    {0xAF, 1},   // xor a
    {0xA8, 1},   // xor b
    {0xA9, 1},   // xor c
    {0xAA, 1},   // xor d
    {0xAB, 1},   // xor e
    {0xAC, 1},   // xor h
    {0xAD, 1},   // xor l
    {0xAE, 1},   // xor (hl)
    {0xEE, 2}    // xor *
};


uint8_t decode_instr_size(uint8_t instr);


#endif /*__INSTR_HPP*/
