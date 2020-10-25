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
#include <utility>
#include "Source.hpp"
#include "Program.hpp"


// Map of pairs. Each pair is <instr, size>
const std::unordered_map<uint32_t, std::pair<uint8_t, uint8_t>> instr_lookup = {
    // (OPCODE << 16)  | (ARG1 << 8) | (ARG2) , (instr, size)
    // add 
    {(INSTR_ADD << 16) | (REG_A       << 8) | REG_A,           std::pair(0x87, 1)},   // add a, a 
    {(INSTR_ADD << 16) | (REG_A       << 8) | REG_B,           std::pair(0x80, 1)},   // add a, b 
    {(INSTR_ADD << 16) | (REG_A       << 8) | REG_C,           std::pair(0x81, 1)},   // add a, c 
    {(INSTR_ADD << 16) | (REG_A       << 8) | REG_D,           std::pair(0x82, 1)},   // add a, d 
    {(INSTR_ADD << 16) | (REG_A       << 8) | REG_E,           std::pair(0x83, 1)},   // add a, e 
    {(INSTR_ADD << 16) | (REG_A       << 8) | REG_H,           std::pair(0x84, 1)},   // add a, h 
    {(INSTR_ADD << 16) | (REG_A       << 8) | REG_L,           std::pair(0x85, 1)},   // add a, l 
    {(INSTR_ADD << 16) | (REG_A       << 8) | REG_HL_IND,      std::pair(0x86, 1)},   // add a, (hl) 
    {(INSTR_ADD << 16) | (REG_A       << 8) | SYM_LITERAL,     std::pair(0xC6, 2)},   // add a, ** 
    {(INSTR_ADD << 16) | (REG_HL      << 8) | REG_BC,          std::pair(0x09, 1)},   // add hl, bc 
    {(INSTR_ADD << 16) | (REG_HL      << 8) | REG_DE,          std::pair(0x19, 1)},   // add hl, de 
    {(INSTR_ADD << 16) | (REG_HL      << 8) | REG_HL,          std::pair(0x29, 1)},   // add hl, hl 
    {(INSTR_ADD << 16) | (REG_HL      << 8) | REG_SP,          std::pair(0x39, 1)},   // add hl, sp 
    // adc
    {(INSTR_ADC << 16) | (REG_A       << 8) | REG_A,           std::pair(0x8F, 1)},   // adc a, a 
    {(INSTR_ADC << 16) | (REG_A       << 8) | REG_B,           std::pair(0x88, 1)},   // adc a, b 
    {(INSTR_ADC << 16) | (REG_A       << 8) | REG_C,           std::pair(0x89, 1)},   // adc a, c 
    {(INSTR_ADC << 16) | (REG_A       << 8) | REG_D,           std::pair(0x8A, 1)},   // adc a, d 
    {(INSTR_ADC << 16) | (REG_A       << 8) | REG_E,           std::pair(0x8B, 1)},   // adc a, e 
    {(INSTR_ADC << 16) | (REG_A       << 8) | REG_H,           std::pair(0x8C, 1)},   // adc a, h 
    {(INSTR_ADC << 16) | (REG_A       << 8) | REG_L,           std::pair(0x8D, 1)},   // adc a, l 
    {(INSTR_ADC << 16) | (REG_A       << 8) | REG_HL_IND,      std::pair(0x8E, 1)},   // adc a, (hl) 
    {(INSTR_ADC << 16) | (REG_A       << 8) | SYM_LITERAL,     std::pair(0xCE, 2)},   // adc a, * 
    // and
    {(INSTR_AND << 16) | (REG_A       << 8),                   std::pair(0xA7, 1)},   // and a
    {(INSTR_AND << 16) | (REG_B       << 8),                   std::pair(0xA0, 1)},   // and b
    {(INSTR_AND << 16) | (REG_C       << 8),                   std::pair(0xA1, 1)},   // and c
    {(INSTR_AND << 16) | (REG_D       << 8),                   std::pair(0xA2, 1)},   // and d
    {(INSTR_AND << 16) | (REG_E       << 8),                   std::pair(0xA3, 1)},   // and e
    {(INSTR_AND << 16) | (REG_H       << 8),                   std::pair(0xA4, 1)},   // and h
    {(INSTR_AND << 16) | (REG_L       << 8),                   std::pair(0xA5, 1)},   // and l
    {(INSTR_AND << 16) | (REG_HL_IND  << 8),                   std::pair(0xA6, 1)},   // and (hl)
    {(INSTR_AND << 16) | (SYM_LITERAL << 8),                   std::pair(0xE6, 2)},   // and *
    // inc 
    {(INSTR_INC << 16) | (REG_A       << 8),                   std::pair(0x3C, 1)},   // inc a
    {(INSTR_INC << 16) | (REG_B       << 8),                   std::pair(0x15, 1)},   // inc b
    {(INSTR_INC << 16) | (REG_C       << 8),                   std::pair(0x0C, 1)},   // inc c
    {(INSTR_INC << 16) | (REG_D       << 8),                   std::pair(0x14, 1)},   // inc d
    {(INSTR_INC << 16) | (REG_E       << 8),                   std::pair(0x1C, 1)},   // inc e
    {(INSTR_INC << 16) | (REG_H       << 8),                   std::pair(0x24, 1)},   // inc h
    {(INSTR_INC << 16) | (REG_L       << 8),                   std::pair(0x2C, 1)},   // inc l
    {(INSTR_INC << 16) | (REG_BC      << 8),                   std::pair(0x03, 1)},   // inc bc
    {(INSTR_INC << 16) | (REG_DE      << 8),                   std::pair(0x13, 1)},   // inc de
    {(INSTR_INC << 16) | (REG_HL      << 8),                   std::pair(0x23, 1)},   // inc hl
    {(INSTR_INC << 16) | (REG_SP      << 8),                   std::pair(0x33, 1)},   // inc sp
    {(INSTR_INC << 16) | (REG_HL_IND  << 8),                   std::pair(0x34, 1)},   // inc (hl)
    // ld (a)
    {(INSTR_LD << 16)  | (REG_A       << 8) | REG_A,           std::pair(0x7F, 1)},   // ld a, a
    {(INSTR_LD << 16)  | (REG_A       << 8) | REG_B,           std::pair(0x78, 1)},   // ld a, b
    {(INSTR_LD << 16)  | (REG_A       << 8) | REG_C,           std::pair(0x79, 1)},   // ld a, c
    {(INSTR_LD << 16)  | (REG_A       << 8) | REG_D,           std::pair(0x7A, 1)},   // ld a, d
    {(INSTR_LD << 16)  | (REG_A       << 8) | REG_E,           std::pair(0x7B, 1)},   // ld a, e
    {(INSTR_LD << 16)  | (REG_A       << 8) | REG_H,           std::pair(0x7C, 1)},   // ld a, h
    {(INSTR_LD << 16)  | (REG_A       << 8) | REG_L,           std::pair(0x7D, 1)},   // ld a, l
    {(INSTR_LD << 16)  | (REG_A       << 8) | REG_HL_IND,      std::pair(0x7E, 1)},   // ld a, (hl)
    {(INSTR_LD << 16)  | (REG_A       << 8) | REG_BC_IND,      std::pair(0x0A, 1)},   // ld a, (bc)
    {(INSTR_LD << 16)  | (REG_A       << 8) | REG_DE_IND,      std::pair(0x1A, 1)},   // ld a, (de)
    {(INSTR_LD << 16)  | (REG_A       << 8) | SYM_LITERAL,     std::pair(0x3E, 2)},   // ld a, *
    {(INSTR_LD << 16)  | (REG_A       << 8) | SYM_LITERAL_IND, std::pair(0x3A, 2)},   // ld a, (**)
    // ld (b)
    {(INSTR_LD << 16)  | (REG_B       << 8) | REG_A,           std::pair(0x06, 1)},   // ld b, a
    {(INSTR_LD << 16)  | (REG_B       << 8) | REG_B,           std::pair(0x40, 1)},   // ld b, b
    {(INSTR_LD << 16)  | (REG_B       << 8) | REG_C,           std::pair(0x41, 1)},   // ld b, c
    {(INSTR_LD << 16)  | (REG_B       << 8) | REG_D,           std::pair(0x42, 1)},   // ld b, d
    {(INSTR_LD << 16)  | (REG_B       << 8) | REG_E,           std::pair(0x43, 1)},   // ld b, e
    {(INSTR_LD << 16)  | (REG_B       << 8) | REG_H,           std::pair(0x44, 1)},   // ld b, h
    {(INSTR_LD << 16)  | (REG_B       << 8) | REG_L,           std::pair(0x45, 1)},   // ld b, l
    {(INSTR_LD << 16)  | (REG_B       << 8) | REG_HL_IND,      std::pair(0x46, 1)},   // ld b, (hl)
    {(INSTR_LD << 16)  | (REG_B       << 8) | SYM_LITERAL,     std::pair(0x06, 2)},   // ld b, *
    // ld (c)
    {(INSTR_LD << 16)  | (REG_C       << 8) | REG_A,           std::pair(0x4F, 1)},   // ld c, a
    {(INSTR_LD << 16)  | (REG_C       << 8) | REG_B,           std::pair(0x48, 1)},   // ld c, b
    {(INSTR_LD << 16)  | (REG_C       << 8) | REG_C,           std::pair(0x49, 1)},   // ld c, c
    {(INSTR_LD << 16)  | (REG_C       << 8) | REG_D,           std::pair(0x4A, 1)},   // ld c, d
    {(INSTR_LD << 16)  | (REG_C       << 8) | REG_E,           std::pair(0x4B, 1)},   // ld c, e
    {(INSTR_LD << 16)  | (REG_C       << 8) | REG_H,           std::pair(0x4C, 1)},   // ld c, h
    {(INSTR_LD << 16)  | (REG_C       << 8) | REG_L,           std::pair(0x4D, 1)},   // ld c, l
    {(INSTR_LD << 16)  | (REG_C       << 8) | REG_HL_IND,      std::pair(0x4E, 1)},   // ld c, (hl)
    {(INSTR_LD << 16)  | (REG_C       << 8) | SYM_LITERAL,     std::pair(0x0E, 2)},   // ld c, *
    // ld (d)
    {(INSTR_LD << 16)  | (REG_D       << 8) | REG_A,           std::pair(0x57, 1)},   // ld d, a
    {(INSTR_LD << 16)  | (REG_D       << 8) | REG_B,           std::pair(0x50, 1)},   // ld d, b
    {(INSTR_LD << 16)  | (REG_D       << 8) | REG_C,           std::pair(0x51, 1)},   // ld d, c
    {(INSTR_LD << 16)  | (REG_D       << 8) | REG_D,           std::pair(0x52, 1)},   // ld d, d
    {(INSTR_LD << 16)  | (REG_D       << 8) | REG_E,           std::pair(0x53, 1)},   // ld d, e
    {(INSTR_LD << 16)  | (REG_D       << 8) | REG_H,           std::pair(0x54, 1)},   // ld d, h
    {(INSTR_LD << 16)  | (REG_D       << 8) | REG_L,           std::pair(0x55, 1)},   // ld d, l
    {(INSTR_LD << 16)  | (REG_D       << 8) | REG_HL_IND,      std::pair(0x56, 1)},   // ld d, (hl)
    {(INSTR_LD << 16)  | (REG_D       << 8) | SYM_LITERAL,     std::pair(0x06, 2)},   // ld d, *
    // ld (e)
    {(INSTR_LD << 16)  | (REG_E       << 8) | REG_A,           std::pair(0x5F, 1)},   // ld e, a
    {(INSTR_LD << 16)  | (REG_E       << 8) | REG_B,           std::pair(0x58, 1)},   // ld e, b
    {(INSTR_LD << 16)  | (REG_E       << 8) | REG_C,           std::pair(0x59, 1)},   // ld e, c
    {(INSTR_LD << 16)  | (REG_E       << 8) | REG_D,           std::pair(0x5A, 1)},   // ld e, d
    {(INSTR_LD << 16)  | (REG_E       << 8) | REG_E,           std::pair(0x5B, 1)},   // ld e, e
    {(INSTR_LD << 16)  | (REG_E       << 8) | REG_H,           std::pair(0x5C, 1)},   // ld e, h
    {(INSTR_LD << 16)  | (REG_E       << 8) | REG_L,           std::pair(0x5D, 1)},   // ld e, l
    {(INSTR_LD << 16)  | (REG_E       << 8) | REG_HL_IND,      std::pair(0x5E, 1)},   // ld e, (hl)
    {(INSTR_LD << 16)  | (REG_E       << 8) | SYM_LITERAL,     std::pair(0x1E, 2)},   // ld e, *
    // ld (h)
    {(INSTR_LD << 16)  | (REG_H       << 8) | REG_A,           std::pair(0x67, 1)},   // ld h, a
    {(INSTR_LD << 16)  | (REG_H       << 8) | REG_B,           std::pair(0x60, 1)},   // ld h, b
    {(INSTR_LD << 16)  | (REG_H       << 8) | REG_C,           std::pair(0x61, 1)},   // ld h, c
    {(INSTR_LD << 16)  | (REG_H       << 8) | REG_D,           std::pair(0x62, 1)},   // ld h, d
    {(INSTR_LD << 16)  | (REG_H       << 8) | REG_E,           std::pair(0x63, 1)},   // ld h, e
    {(INSTR_LD << 16)  | (REG_H       << 8) | REG_H,           std::pair(0x64, 1)},   // ld h, h
    {(INSTR_LD << 16)  | (REG_H       << 8) | REG_L,           std::pair(0x65, 1)},   // ld h, l
    {(INSTR_LD << 16)  | (REG_H       << 8) | REG_HL_IND,      std::pair(0x66, 1)},   // ld h, (hl)
    {(INSTR_LD << 16)  | (REG_H       << 8) | SYM_LITERAL,     std::pair(0x26, 2)},   // ld h, *
    // ld (l)
    {(INSTR_LD << 16)  | (REG_L       << 8) | REG_A,           std::pair(0x6F, 1)},   // ld l, a
    {(INSTR_LD << 16)  | (REG_L       << 8) | REG_B,           std::pair(0x68, 1)},   // ld l, b
    {(INSTR_LD << 16)  | (REG_L       << 8) | REG_C,           std::pair(0x69, 1)},   // ld l, c
    {(INSTR_LD << 16)  | (REG_L       << 8) | REG_D,           std::pair(0x6A, 1)},   // ld l, d
    {(INSTR_LD << 16)  | (REG_L       << 8) | REG_E,           std::pair(0x6B, 1)},   // ld l, e
    {(INSTR_LD << 16)  | (REG_L       << 8) | REG_H,           std::pair(0x6C, 1)},   // ld l, h
    {(INSTR_LD << 16)  | (REG_L       << 8) | REG_L,           std::pair(0x6D, 1)},   // ld l, l
    {(INSTR_LD << 16)  | (REG_L       << 8) | REG_HL_IND,      std::pair(0x6E, 1)},   // ld l, (hl)
    {(INSTR_LD << 16)  | (REG_L       << 8) | SYM_LITERAL,     std::pair(0x2E, 2)},   // ld l, *
    // ld (indrections)
    {(INSTR_LD << 16)  | (REG_HL_IND  << 8) | REG_A,           std::pair(0x77, 1)},   // ld (hl), a
    {(INSTR_LD << 16)  | (REG_HL_IND  << 8) | REG_B,           std::pair(0x70, 1)},   // ld (hl), b
    {(INSTR_LD << 16)  | (REG_HL_IND  << 8) | REG_C,           std::pair(0x71, 1)},   // ld (hl), c
    {(INSTR_LD << 16)  | (REG_HL_IND  << 8) | REG_D,           std::pair(0x72, 1)},   // ld (hl), d
    {(INSTR_LD << 16)  | (REG_HL_IND  << 8) | REG_H,           std::pair(0x73, 1)},   // ld (hl), e
    {(INSTR_LD << 16)  | (REG_HL_IND  << 8) | REG_H,           std::pair(0x74, 1)},   // ld (hl), h
    {(INSTR_LD << 16)  | (REG_HL_IND  << 8) | REG_L,           std::pair(0x75, 1)},   // ld (hl), l
    {(INSTR_LD << 16)  | (REG_HL_IND  << 8) | REG_HL_IND,      std::pair(0x76, 1)},   // ld (hl), (hl)
    {(INSTR_LD << 16)  | (REG_HL_IND  << 8) | SYM_LITERAL,     std::pair(0x36, 2)},   // ld (hl), *
    {(INSTR_LD << 16)  | (REG_BC_IND  << 8) | REG_A,           std::pair(0x02, 1)},   // ld (bc), a
    {(INSTR_LD << 16)  | (REG_DE_IND  << 8) | REG_A,           std::pair(0x12, 1)},   // ld (de), a
    // ld (**)
    {(INSTR_LD << 16)  | (SYM_LITERAL_IND << 8) | REG_HL,      std::pair(0x22, 3)},   // ld (**), hl

    {(INSTR_LD << 16)  | (REG_BC      << 8) | SYM_LITERAL,     std::pair(0x01, 3)},   // ld bc, **
    {(INSTR_LD << 16)  | (REG_DE      << 8) | SYM_LITERAL,     std::pair(0x11, 3)},   // ld de, **
    {(INSTR_LD << 16)  | (REG_HL      << 8) | SYM_LITERAL,     std::pair(0x21, 3)},   // ld hl, **
    {(INSTR_LD << 16)  | (REG_SP      << 8) | SYM_LITERAL,     std::pair(0x31, 3)},   // ld sp, **

    // or 
    {(INSTR_OR << 16)  | (REG_A       << 8),                   std::pair(0xB7, 1)},   // or a
    {(INSTR_OR << 16)  | (REG_B       << 8),                   std::pair(0xB0, 1)},   // or b
    {(INSTR_OR << 16)  | (REG_C       << 8),                   std::pair(0xB1, 1)},   // or c
    {(INSTR_OR << 16)  | (REG_D       << 8),                   std::pair(0xB2, 1)},   // or d
    {(INSTR_OR << 16)  | (REG_E       << 8),                   std::pair(0xB3, 1)},   // or e
    {(INSTR_OR << 16)  | (REG_H       << 8),                   std::pair(0xB4, 1)},   // or h
    {(INSTR_OR << 16)  | (REG_L       << 8),                   std::pair(0xB5, 1)},   // or l
    {(INSTR_OR << 16)  | (REG_HL_IND  << 8),                   std::pair(0xB6, 1)},   // or (hl)
    {(INSTR_OR << 16)  | (SYM_LITERAL << 8),                   std::pair(0xF6, 2)},   // or *
    // ret
    {(INSTR_RET << 16)  | (COND_NZ    << 8),                   std::pair(0xC0, 1)},   // ret nz
    {(INSTR_RET << 16)  | (COND_NC    << 8),                   std::pair(0xD0, 1)},   // ret nc
    {(INSTR_RET << 16)  | (COND_PO    << 8),                   std::pair(0xE0, 1)},   // ret po
    {(INSTR_RET << 16)  | (COND_P     << 8),                   std::pair(0xF0, 1)},   // ret p
    {(INSTR_RET << 16)  | (COND_Z     << 8),                   std::pair(0xC8, 1)},   // ret z
    {(INSTR_RET << 16)  | (COND_C     << 8),                   std::pair(0xD8, 1)},   // ret c
    {(INSTR_RET << 16)  | (COND_PE    << 8),                   std::pair(0xE8, 1)},   // ret pe
    {(INSTR_RET << 16)  | (COND_M     << 8),                   std::pair(0xF8, 1)},   // ret m
    {(INSTR_RET << 16)  ,                                      std::pair(0xC9, 1)},   // ret 
    // sbc
    {(INSTR_SBC << 16) | (REG_A       << 8) | REG_A,           std::pair(0x9F, 1)},   // sbc a, a 
    {(INSTR_SBC << 16) | (REG_A       << 8) | REG_B,           std::pair(0x98, 1)},   // sbc a, b 
    {(INSTR_SBC << 16) | (REG_A       << 8) | REG_C,           std::pair(0x99, 1)},   // sbc a, c 
    {(INSTR_SBC << 16) | (REG_A       << 8) | REG_D,           std::pair(0x9A, 1)},   // sbc a, d 
    {(INSTR_SBC << 16) | (REG_A       << 8) | REG_E,           std::pair(0x9B, 1)},   // sbc a, e 
    {(INSTR_SBC << 16) | (REG_A       << 8) | REG_H,           std::pair(0x9C, 1)},   // sbc a, h 
    {(INSTR_SBC << 16) | (REG_A       << 8) | REG_L,           std::pair(0x9D, 1)},   // sbc a, l 
    {(INSTR_SBC << 16) | (REG_A       << 8) | REG_HL_IND,      std::pair(0x9E, 1)},   // sbc a, (hl) 
    {(INSTR_SBC << 16) | (REG_A       << 8) | SYM_LITERAL,     std::pair(0xDE, 2)},   // sbc a, * 
    // sub
    {(INSTR_SUB << 16) | (REG_A       << 8),                   std::pair(0x97, 1)},   // sub a
    {(INSTR_SUB << 16) | (REG_B       << 8),                   std::pair(0x90, 1)},   // sub b
    {(INSTR_SUB << 16) | (REG_C       << 8),                   std::pair(0x91, 1)},   // sub c
    {(INSTR_SUB << 16) | (REG_D       << 8),                   std::pair(0x92, 1)},   // sub d
    {(INSTR_SUB << 16) | (REG_E       << 8),                   std::pair(0x93, 1)},   // sub e
    {(INSTR_SUB << 16) | (REG_H       << 8),                   std::pair(0x94, 1)},   // sub h
    {(INSTR_SUB << 16) | (REG_L       << 8),                   std::pair(0x95, 1)},   // sub l
    {(INSTR_SUB << 16) | (REG_HL_IND  << 8),                   std::pair(0x96, 1)},   // sub (hl)
    {(INSTR_SUB << 16) | (SYM_LITERAL << 8),                   std::pair(0xD6, 2)},   // sub *
    // xor
    {(INSTR_XOR << 16) | (REG_A       << 8),                   std::pair(0xAF, 1)},   // xor a
    {(INSTR_XOR << 16) | (REG_B       << 8),                   std::pair(0xA8, 1)},   // xor b
    {(INSTR_XOR << 16) | (REG_C       << 8),                   std::pair(0xA9, 1)},   // xor c
    {(INSTR_XOR << 16) | (REG_D       << 8),                   std::pair(0xAA, 1)},   // xor d
    {(INSTR_XOR << 16) | (REG_E       << 8),                   std::pair(0xAB, 1)},   // xor e
    {(INSTR_XOR << 16) | (REG_H       << 8),                   std::pair(0xAC, 1)},   // xor h
    {(INSTR_XOR << 16) | (REG_L       << 8),                   std::pair(0xAD, 1)},   // xor l
    {(INSTR_XOR << 16) | (REG_HL_IND  << 8),                   std::pair(0xAE, 1)},   // xor (hl)
    {(INSTR_XOR << 16) | (SYM_LITERAL << 8),                   std::pair(0xEE, 2)}    // xor *
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
