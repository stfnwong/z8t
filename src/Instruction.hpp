/*
 * INSTRUCTION
 * Lookups for instructions
 *
 */


#ifndef __INSTRUCTION_HPP
#define __INSTRUCTION_HPP

#include <map>
#include <unordered_map>
#include <utility>

#include "Source.hpp"


// Map of pairs. Each pair is <instr, size>
const std::unordered_map<uint32_t, std::pair<uint8_t, uint8_t> > instr_hash_to_code = {
    // format 
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
    // cp 
    {(INSTR_CP  << 16) | (REG_A       << 8),                   std::pair(0xBF, 1)},   // cp a
    {(INSTR_CP  << 16) | (REG_B       << 8),                   std::pair(0xB8, 1)},   // cp b
    {(INSTR_CP  << 16) | (REG_C       << 8),                   std::pair(0xB9, 1)},   // cp c
    {(INSTR_CP  << 16) | (REG_D       << 8),                   std::pair(0xBA, 1)},   // cp d
    {(INSTR_CP  << 16) | (REG_E       << 8),                   std::pair(0xBB, 1)},   // cp e
    {(INSTR_CP  << 16) | (REG_H       << 8),                   std::pair(0xBC, 1)},   // cp h
    {(INSTR_CP  << 16) | (REG_L       << 8),                   std::pair(0xBD, 1)},   // cp l
    {(INSTR_CP  << 16) | (REG_HL      << 8),                   std::pair(0xBE, 1)},   // cp (hl)
    {(INSTR_CP  << 16) | (SYM_LITERAL << 8),                   std::pair(0xFE, 2)},   // cp *
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
    // jr 
    {(INSTR_JR  << 16) | (SYM_LITERAL << 8),                   std::pair(0x18, 2)},   // jr *
    {(INSTR_JR  << 16) | (COND_Z      << 8) | SYM_LITERAL,     std::pair(0x28, 2)},   // jr z, *
    {(INSTR_JR  << 16) | (COND_C      << 8) | SYM_LITERAL,     std::pair(0x38, 2)},   // jr c, *
    {(INSTR_JR  << 16) | (COND_NZ     << 8) | SYM_LITERAL,     std::pair(0x20, 2)},   // jr nz, *
    {(INSTR_JR  << 16) | (COND_NC     << 8) | SYM_LITERAL,     std::pair(0x30, 2)},   // jr nc, *
    // jp 
    {(INSTR_JP  << 16) | (COND_NZ     << 8) | SYM_LITERAL,     std::pair(0xC3, 3)},   // jp nz, **
    {(INSTR_JP  << 16) | (COND_NC     << 8) | SYM_LITERAL,     std::pair(0xD3, 3)},   // jp nc, **
    {(INSTR_JP  << 16) | (COND_PO     << 8) | SYM_LITERAL,     std::pair(0xE3, 3)},   // jp po, **
    {(INSTR_JP  << 16) | (COND_P      << 8) | SYM_LITERAL,     std::pair(0xF3, 3)},   // jp p, **
    {(INSTR_JP  << 16) | (SYM_LITERAL << 8),                   std::pair(0xC4, 3)},   // jp **
    {(INSTR_JP  << 16) | (REG_HL_IND  << 8),                   std::pair(0xE9, 3)},   // jp (hl)
    {(INSTR_JP  << 16) | (COND_Z      << 8) | SYM_LITERAL,     std::pair(0xCA, 3)},   // jp z, **
    {(INSTR_JP  << 16) | (COND_C      << 8) | SYM_LITERAL,     std::pair(0xDA, 3)},   // jp c, **
    {(INSTR_JP  << 16) | (COND_PE     << 8) | SYM_LITERAL,     std::pair(0xEA, 3)},   // jp pe, **
    {(INSTR_JP  << 16) | (COND_M      << 8) | SYM_LITERAL,     std::pair(0xFA, 3)},   // jp m, **
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
    {(INSTR_LD << 16)  | (REG_B       << 8) | REG_A,           std::pair(0x47, 1)},   // ld b, a
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
    // ld (hl)
    {(INSTR_LD << 16)  | (REG_HL      << 8) | SYM_LITERAL,     std::pair(0x21, 3)},   // ld hl, **
    {(INSTR_LD << 16)  | (REG_HL      << 8) | SYM_LITERAL_IND, std::pair(0x2A, 3)},   // ld hl, (**)
    // ld (indrections)
    {(INSTR_LD << 16)  | (REG_HL_IND  << 8) | REG_A,           std::pair(0x77, 1)},   // ld (hl), a
    {(INSTR_LD << 16)  | (REG_HL_IND  << 8) | REG_B,           std::pair(0x70, 1)},   // ld (hl), b
    {(INSTR_LD << 16)  | (REG_HL_IND  << 8) | REG_C,           std::pair(0x71, 1)},   // ld (hl), c
    {(INSTR_LD << 16)  | (REG_HL_IND  << 8) | REG_D,           std::pair(0x72, 1)},   // ld (hl), d
    {(INSTR_LD << 16)  | (REG_HL_IND  << 8) | REG_E,           std::pair(0x73, 1)},   // ld (hl), e
    {(INSTR_LD << 16)  | (REG_HL_IND  << 8) | REG_H,           std::pair(0x74, 1)},   // ld (hl), h
    {(INSTR_LD << 16)  | (REG_HL_IND  << 8) | REG_L,           std::pair(0x75, 1)},   // ld (hl), l
    {(INSTR_LD << 16)  | (REG_HL_IND  << 8) | REG_HL_IND,      std::pair(0x76, 1)},   // ld (hl), (hl)
    {(INSTR_LD << 16)  | (REG_HL_IND  << 8) | SYM_LITERAL,     std::pair(0x36, 2)},   // ld (hl), *
    {(INSTR_LD << 16)  | (REG_BC_IND  << 8) | REG_A,           std::pair(0x02, 1)},   // ld (bc), a
    {(INSTR_LD << 16)  | (REG_DE_IND  << 8) | REG_A,           std::pair(0x12, 1)},   // ld (de), a
    // ld (**)
    {(INSTR_LD << 16)  | (SYM_LITERAL_IND << 8) | REG_HL,      std::pair(0x22, 3)},   // ld (**), hl
    // ld 
    {(INSTR_LD << 16)  | (REG_BC      << 8) | SYM_LITERAL,     std::pair(0x01, 3)},   // ld bc, **
    {(INSTR_LD << 16)  | (REG_DE      << 8) | SYM_LITERAL,     std::pair(0x11, 3)},   // ld de, **
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
    // pop
    {(INSTR_POP << 16)  | (REG_BC     << 8),                   std::pair(0xC1, 1)},   // pop bc
    {(INSTR_POP << 16)  | (REG_DE     << 8),                   std::pair(0xD1, 1)},   // pop de
    {(INSTR_POP << 16)  | (REG_HL     << 8),                   std::pair(0xE1, 1)},   // pop hl
    {(INSTR_POP << 16)  | (REG_AF     << 8),                   std::pair(0xF1, 1)},   // pop af
    // push
    {(INSTR_PUSH << 16) | (REG_BC     << 8),                   std::pair(0xC5, 1)},   // push bc
    {(INSTR_PUSH << 16) | (REG_DE     << 8),                   std::pair(0xD5, 1)},   // push de
    {(INSTR_PUSH << 16) | (REG_HL     << 8),                   std::pair(0xE5, 1)},   // push hl
    {(INSTR_PUSH << 16) | (REG_AF     << 8),                   std::pair(0xF5, 1)},   // push af
    // ret
    {(INSTR_RET << 16)  | (COND_NZ    << 8),                   std::pair(0xC0, 1)},   // ret nz
    {(INSTR_RET << 16)  | (COND_NC    << 8),                   std::pair(0xD0, 1)},   // ret nc
    {(INSTR_RET << 16)  | (COND_PO    << 8),                   std::pair(0xE0, 1)},   // ret po
    {(INSTR_RET << 16)  | (COND_P     << 8),                   std::pair(0xF0, 1)},   // ret p
    {(INSTR_RET << 16)  | (COND_Z     << 8),                   std::pair(0xC8, 1)},   // ret z
    {(INSTR_RET << 16)  | (COND_C     << 8),                   std::pair(0xD8, 1)},   // ret c
    {(INSTR_RET << 16)  | (COND_PE    << 8),                   std::pair(0xE8, 1)},   // ret pe
    {(INSTR_RET << 16)  | (COND_M     << 8),                   std::pair(0xF8, 1)},   // ret m
    {(INSTR_RET << 16)                     ,                   std::pair(0xC9, 1)},   // ret 
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

// Map of pairs. Each pair is <repr, size>
// FIXME: This doesn't work with an unordered_map due to errors 
// in the hashing function. This seems absurd at face value since 
// the key here is just a uint8_t.
const std::map<const uint8_t, std::pair<std::string, uint8_t>> code_to_instr_repr = {
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
    {0x47, std::pair("ld b, a", 1)},      // ld b, a
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
 * Find the size of an instruction in bytes given its hash
 */
uint8_t instr_get_size(uint32_t arg_hash);

#endif /*__INSTRUCTION_HPP*/
