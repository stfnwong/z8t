/*
 * DISASSEMBLER
 *
 */

#ifndef __DISASSEMBLER_HPP
#define __DISASSEMBLER_HPP

#include <string>
#include <unordered_map>
#include <utility>
#include "Source.hpp"


// TODO: need to rename this map
// Map of pairs. Each pair is <repr, size>
const std::unordered_map<uint8_t, std::pair<std::string, uint8_t>> dis_instr_lookup = {
    // format 
    // <First byte of instr> -> (repr, size)
    // add 
    {0x87, std::pair("add a, a", 1)},     // add a, a 
    {0x80, std::pair("add a, b", 1)},     // add a, b 
    {0x81, std::pair("add a, c", 1)},     // add a, c 
    {0x82, std::pair("add a, d", 1)},     // add a, d 
    {0x83, std::pair("add a, e", 1)},     // add a, e 
    {0x84, std::pair("add a, h", 1)},     // add a, h 
    {0x85, std::pair("add a, l", 1)},     // add a, l 
    {0x86, std::pair("add a, (hl)", 1)},  // add a, (hl) 
    {0xC6, std::pair("add a,", 2)},       // add a, **  (extend with literal)
    {0x09, std::pair("add hl, bc", 1)},   // add hl, bc 
    {0x19, std::pair("add hl, de", 1)},   // add hl, de 
    {0x29, std::pair("add hl, hl", 1)},   // add hl, hl 
    {0x39, std::pair("add hl, sp", 1)},   // add hl, sp 
    // adc
    {0x8F, std::pair("adc a, a", 1)},     // adc a, a 
    {0x88, std::pair("adc a, b", 1)},     // adc a, b 
    {0x89, std::pair("adc a, c", 1)},     // adc a, c 
    {0x8A, std::pair("adc a, d", 1)},     // adc a, d 
    {0x8B, std::pair("adc a, e", 1)},     // adc a, e 
    {0x8C, std::pair("adc a, h", 1)},     // adc a, h 
    {0x8D, std::pair("adc a, l", 1)},     // adc a, l 
    {0x8E, std::pair("adc a, (hl)", 1)},  // adc a, (hl) 
    {0xCE, std::pair("adc a,", 2)},       // adc a, * 
    // and
    {0xA7, std::pair("and a", 1)},        // and a
    {0xA0, std::pair("and b", 1)},        // and b
    {0xA1, std::pair("and c", 1)},        // and c
    {0xA2, std::pair("and d", 1)},        // and d
    {0xA3, std::pair("and e", 1)},        // and e
    {0xA4, std::pair("and h", 1)},        // and h
    {0xA5, std::pair("and l", 1)},        // and l
    {0xA6, std::pair("and (hl)", 1)},     // and (hl)
    {0xE6, std::pair("and ", 2)},         // and *
    // cp 
    {0xBF, std::pair("cp a", 1)},         // cp a
    {0xB8, std::pair("cp b", 1)},         // cp b
    {0xB9, std::pair("cp c", 1)},         // cp c
    {0xBA, std::pair("cp d", 1)},         // cp d
    {0xBB, std::pair("cp e", 1)},         // cp e
    {0xBC, std::pair("cp h", 1)},         // cp h
    {0xBD, std::pair("cp l", 1)},         // cp l
    {0xBE, std::pair("cp (hl)", 1)},      // cp (hl)
    {0xFE, std::pair("cp ", 2)},          // cp *
    // inc 
    {0x3C, std::pair("inc a", 1)},        // inc a
    {0x15, std::pair("inc b", 1)},        // inc b
    {0x0C, std::pair("inc c", 1)},        // inc c
    {0x14, std::pair("inc d", 1)},        // inc d
    {0x1C, std::pair("inc e", 1)},        // inc e
    {0x24, std::pair("inc h", 1)},        // inc h
    {0x2C, std::pair("inc l", 1)},        // inc l
    {0x03, std::pair("inc bc", 1)},       // inc bc
    {0x13, std::pair("inc de", 1)},       // inc de
    {0x23, std::pair("inc hl", 1)},       // inc hl
    {0x33, std::pair("inc sp", 1)},       // inc sp
    {0x34, std::pair("inc (hl)", 1)},     // inc (hl)
    // jr 
    {0x18, std::pair("jr", 2)},           // jr *
    {0x28, std::pair("jr z", 2)},         // jr z, *
    {0x38, std::pair("jr c", 2)},         // jr c, *
    {0x20, std::pair("jr nz", 2)},        // jr nz, *
    {0x30, std::pair("jr nc", 2)},        // jr nc, *
    // jp 
    {0xC3, std::pair("jp nz,", 3)},       // jp nz, **
    {0xD3, std::pair("jp nc,", 3)},       // jp nc, **
    {0xE3, std::pair("jp po,", 3)},       // jp po, **
    {0xF3, std::pair("jp p,", 3)},        // jp p, **
    {0xC4, std::pair("jp nz,", 3)},       // jp **
    {0xE9, std::pair("jp (hl),", 3)},     // jp (hl)
    {0xCA, std::pair("jp z,", 3)},        // jp z, **
    {0xDA, std::pair("jp c,", 3)},        // jp c, **
    {0xEA, std::pair("jp pe,", 3)},       // jp pe, **
    {0xFA, std::pair("jp m,", 3)},        // jp m, **
    // ld (a)
    {0x7F, std::pair("ld a, a", 1)},      // ld a, a
    {0x78, std::pair("ld a, b", 1)},      // ld a, b
    {0x79, std::pair("ld a, c", 1)},      // ld a, c
    {0x7A, std::pair("ld a, d", 1)},      // ld a, d
    {0x7B, std::pair("ld a, e", 1)},      // ld a, e
    {0x7C, std::pair("ld a, h", 1)},      // ld a, h
    {0x7D, std::pair("ld a, l", 1)},      // ld a, l
    {0x7E, std::pair("ld a, (hl)", 1)},   // ld a, (hl)
    {0x0A, std::pair("ld a, (bc)", 1)},   // ld a, (bc)
    {0x1A, std::pair("ld a, (de)", 1)},   // ld a, (de)
    {0x3E, std::pair("ld a, ", 2)},       // ld a, *
    {0x3A, std::pair("ld a, ", 2)},       // ld a, (**)
    // ld (b)
    {0x06, std::pair("ld b, a", 1)},      // ld b, a
    {0x40, std::pair("ld b, b", 1)},      // ld b, b
    {0x41, std::pair("ld b, c", 1)},      // ld b, c
    {0x42, std::pair("ld b, d", 1)},      // ld b, d
    {0x43, std::pair("ld b, e", 1)},      // ld b, e
    {0x44, std::pair("ld b, h", 1)},      // ld b, h
    {0x45, std::pair("ld b, l", 1)},      // ld b, l
    {0x46, std::pair("ld b, (hl)", 1)},   // ld b, (hl)
    {0x06, std::pair("ld b, ", 2)},       // ld b, *
    // ld (c)
    {0x4F, std::pair("ld c, a", 1)},      // ld c, a
    {0x48, std::pair("ld c, b", 1)},      // ld c, b
    {0x49, std::pair("ld c, c", 1)},      // ld c, c
    {0x4A, std::pair("ld c, d", 1)},      // ld c, d
    {0x4B, std::pair("ld c, e", 1)},      // ld c, e
    {0x4C, std::pair("ld c, h", 1)},      // ld c, h
    {0x4D, std::pair("ld c, l", 1)},      // ld c, l
    {0x4E, std::pair("ld c, (hl)", 1)},   // ld c, (hl)
    {0x0E, std::pair("ld c, ", 2)},       // ld c, *
    // ld (d)
    {0x57, std::pair("ld d, a", 1)},      // ld d, a
    {0x50, std::pair("ld d, b", 1)},      // ld d, b
    {0x51, std::pair("ld d, c", 1)},      // ld d, c
    {0x52, std::pair("ld d, d", 1)},      // ld d, d
    {0x53, std::pair("ld d, e", 1)},      // ld d, e
    {0x54, std::pair("ld d, h", 1)},      // ld d, h
    {0x55, std::pair("ld d, l", 1)},      // ld d, l
    {0x56, std::pair("ld d, (hl)", 1)},   // ld d, (hl)
    {0x06, std::pair("ld d, ", 2)},       // ld d, *
    // ld (e)
    {0x5F, std::pair("ld e, a", 1)},      // ld e, a
    {0x58, std::pair("ld e, b", 1)},      // ld e, b
    {0x59, std::pair("ld e, c", 1)},      // ld e, c
    {0x5A, std::pair("ld e, d", 1)},      // ld e, d
    {0x5B, std::pair("ld e, e", 1)},      // ld e, e
    {0x5C, std::pair("ld e, h", 1)},      // ld e, h
    {0x5D, std::pair("ld e, l", 1)},      // ld e, l
    {0x5E, std::pair("ld e, (hl)", 1)},   // ld e, (hl)
    {0x1E, std::pair("ld e, ", 2)},       // ld e, *
    // ld (h)
    {0x67, std::pair("ld h, a", 1)},      // ld h, a
    {0x60, std::pair("ld h, b", 1)},      // ld h, b
    {0x61, std::pair("ld h, c", 1)},      // ld h, c
    {0x62, std::pair("ld h, d", 1)},      // ld h, d
    {0x63, std::pair("ld h, e", 1)},      // ld h, e
    {0x64, std::pair("ld h, h", 1)},      // ld h, h
    {0x65, std::pair("ld h, l", 1)},      // ld h, l
    {0x66, std::pair("ld h, (hl)", 1)},   // ld h, (hl)
    {0x26, std::pair("ld h, ", 2)},       // ld h, *
    // ld (l)
    {0x6F, std::pair("ld l, a", 1)},      // ld l, a
    {0x68, std::pair("ld l, b", 1)},      // ld l, b
    {0x69, std::pair("ld l, c", 1)},      // ld l, c
    {0x6A, std::pair("ld l, d", 1)},      // ld l, d
    {0x6B, std::pair("ld l, e", 1)},      // ld l, e
    {0x6C, std::pair("ld l, h", 1)},      // ld l, h
    {0x6D, std::pair("ld l, l", 1)},      // ld l, l
    {0x6E, std::pair("ld l, (hl)", 1)},   // ld l, (hl)
    {0x2E, std::pair("ld l, ", 2)},       // ld l, *
    // ld (indrections)
    {0x77, std::pair("ld (hl), a", 1)},   // ld (hl), a
    {0x70, std::pair("ld (hl), b", 1)},   // ld (hl), b
    {0x71, std::pair("ld (hl), c", 1)},   // ld (hl), c
    {0x72, std::pair("ld (hl), d", 1)},   // ld (hl), d
    {0x73, std::pair("ld (hl), e", 1)},   // ld (hl), e
    {0x74, std::pair("ld (hl), h", 1)},   // ld (hl), h
    {0x75, std::pair("ld (hl), l", 1)},   // ld (hl), l
    {0x76, std::pair("ld (hl), (hl)", 1)},// ld (hl), (hl)
    {0x36, std::pair("ld (hl), ", 2)},    // ld (hl), *
    {0x02, std::pair("ld (bc), a", 1)},   // ld (bc), a
    {0x12, std::pair("ld (de), a", 1)},   // ld (de), a
    // ld (**)
    {0x22, std::pair("ld (**), hl,", 3)}, // ld (**), hl      // TODO: figure out what to do in this case wrt. string formatting
    {0x01, std::pair("ld bc,", 3)},       // ld bc, **
    {0x11, std::pair("ld de,", 3)},       // ld de, **
    {0x21, std::pair("ld hl,", 3)},       // ld hl, **
    {0x31, std::pair("ld sp,", 3)},       // ld sp, **
    // or 
    {0xB7, std::pair("or a", 1)},         // or a
    {0xB0, std::pair("or b", 1)},         // or b
    {0xB1, std::pair("or c", 1)},         // or c
    {0xB2, std::pair("or d", 1)},         // or d
    {0xB3, std::pair("or e", 1)},         // or e
    {0xB4, std::pair("or h", 1)},         // or h
    {0xB5, std::pair("or l", 1)},         // or l
    {0xB6, std::pair("or (hl)", 1)},      // or (hl)
    {0xF6, std::pair("or ", 2)},          // or *
    // ret
    {0xC0, std::pair("ret nz", 1)},       // ret nz
    {0xD0, std::pair("ret nc", 1)},       // ret nc
    {0xE0, std::pair("ret po", 1)},       // ret po
    {0xF0, std::pair("ret p", 1)},        // ret p
    {0xC8, std::pair("ret z", 1)},        // ret z
    {0xD8, std::pair("ret c", 1)},        // ret c
    {0xE8, std::pair("ret pe", 1)},       // ret pe
    {0xF8, std::pair("ret m", 1)},        // ret m
    {0xC9, std::pair("ret ", 1)},         // ret 
    // sbc
    {0x9F, std::pair("sbc a. a", 1)},     // sbc a, a 
    {0x98, std::pair("sbc a. b", 1)},     // sbc a, b 
    {0x99, std::pair("sbc a. c", 1)},     // sbc a, c 
    {0x9A, std::pair("sbc a. d", 1)},     // sbc a, d 
    {0x9B, std::pair("sbc a. e", 1)},     // sbc a, e 
    {0x9C, std::pair("sbc a. h", 1)},     // sbc a, h 
    {0x9D, std::pair("sbc a. l", 1)},     // sbc a, l 
    {0x9E, std::pair("sbc a. (hl)", 1)},  // sbc a, (hl) 
    {0xDE, std::pair("sbc a. ", 2)},      // sbc a, * 
    // sub
    {0x97, std::pair("sub a", 1)},        // sub a
    {0x90, std::pair("sub b", 1)},        // sub b
    {0x91, std::pair("sub c", 1)},        // sub c
    {0x92, std::pair("sub d", 1)},        // sub d
    {0x93, std::pair("sub e", 1)},        // sub e
    {0x94, std::pair("sub h", 1)},        // sub h
    {0x95, std::pair("sub l", 1)},        // sub l
    {0x96, std::pair("sub (hl)", 1)},     // sub (hl)
    {0xD6, std::pair("sub ", 2)},         // sub *
    // xor
    {0xAF, std::pair("xor a", 1)},        // xor a
    {0xA8, std::pair("xor b", 1)},        // xor b
    {0xA9, std::pair("xor c", 1)},        // xor c
    {0xAA, std::pair("xor d", 1)},        // xor d
    {0xAB, std::pair("xor e", 1)},        // xor e
    {0xAC, std::pair("xor h", 1)},        // xor h
    {0xAD, std::pair("xor l", 1)},        // xor l
    {0xAE, std::pair("xor (hl)", 1)},     // xor (hl)
    {0xEE, std::pair("xor ", 2)}          // xor *
};


/*
 * dis_program()
 * Give a codebuffer stream, consume bytes and emit a TextLine 
 * that represents the given instruction
 */
TextLine dis_program(const uint8_t *code_buffer, int pc);


#endif /*__DISASSEMBLER_HPP*/
