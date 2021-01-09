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

// TODO : put this with the other instruction maps
// Map of pairs. Each pair is <instr, size>
const std::unordered_map<uint32_t, std::pair<uint8_t, uint8_t>> instr_lookup = {
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
    {(INSTR_INC << 16) | (REG_C       << 8),                   std::pair(0X0C, 1)},   // inc c
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


/*
 * get the size of an instruction from the lookup table
 */
uint8_t instr_get_size(uint32_t arg_hash);

/*
 * Assembler 
 * Takes source text and transforms into a Program object 
 * which can be written out as a program executable.
 */
class Assembler
{
    private:
        bool verbose;
        Program program;

    // input source text
    private:
        void init_mem(void);    // create token buffer memory
        std::string source;
        std::string filename;

    // source intermediate representation
    private:
        SourceInfo   source_info;
        LineInfo     line_info;
        Token        cur_token;
        unsigned int cur_addr;

    // token lookup
    private:
        TokenLookup token_lookup;
        OpcodeLookup opcode_lookup;

    // source movement 
    private:
        bool exhausted(void);
        void advance(void);
        unsigned int cur_pos;
        unsigned int cur_line;
        char         cur_char;
        char*        token_buf;
        unsigned int token_buf_size;

    // token check 
    private:
        bool is_space(void);
        bool is_comment(void);

    // parsing 
    private:
        Token       tok_string_to_literal(const std::string& tok_string) const;
        Token       next_token(void);

        void        skip_whitespace(void);
        void        skip_line(void);
        void        skip_seperators(void);
        void        skip_to_next_token(void);
        void        scan_token(void);
        void        parse_arg(int arg_idx);
        void        parse_one_or_two_arg(void);
        void        parse_instruction(const Token& token);
        std::string read_to_line_end(void);
        void        parse_directive(const Token& token); 
        void        parse_line(void);
        void        resolve_labels(void);

    // assemble from intermediate repr
    private:
        void assem_instr(void);

    public:
        Assembler();
        Assembler(const Assembler& that) = delete;
        Assembler(const Assembler&& that) = delete;

        void  init(void);
        void  assemble(void);
        void  setVerbose(const bool v);
        bool  getVerbose(void) const;
        int   read(const std::string& filename);
        void  loadSource(const std::string& src);
        const SourceInfo& getSourceInfo(void) const;
        unsigned int getCurAddr(void) const;

        // getters, setters 
        Program getProgram(void) const;

        // TODO: debug, remove 
        void printSource(void) const;
};

#endif /*__ASSEMBLER_HPP*/
