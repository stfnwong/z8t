/*
 * CPU
 * Components in the Z80 CPU
 */

#include <cstring>
#include <iostream>
#include <iomanip>

#include "CPU.hpp"
#include "Instr.hpp"


/*
 * MEMORY
 */

Memory::Memory(unsigned int s) : mem_size(s) 
{
    this->data = new uint8_t[this->mem_size];
}

Memory::Memory(const Memory& that) 
{
    this->mem_size = that.mem_size;
    this->data = new uint8_t[this->mem_size];
    std::memcpy(this->data, that.data, this->mem_size);
}

Memory::~Memory()
{
    delete[] this->data;
}


uint8_t& Memory::operator[](unsigned int i)
{
    return this->data[i];
}

const uint8_t Memory::operator[](unsigned int i) const
{
    return this->data[i];
}

// NOTE: slow!
bool Memory::operator==(const Memory& that) const
{
    if(this->mem_size != that.mem_size)
        return false;
    
    for(unsigned int i = 0; i < this->mem_size; ++i)
    {
        if(this->data[i] != that.data[i])
            return false;
    }

    return true;
}

bool Memory::operator!=(const Memory& that) const
{
    return !(*this == that);
}

void Memory::clear(void)
{
    std::memset(this->data, 0, this->mem_size);
}

unsigned int Memory::size(void) const
{
    return this->mem_size;
}

void Memory::load(const uint8_t* data, unsigned int n, unsigned int offset)
{
    if((n + offset) > this->size())
        return;

    for(unsigned int i = 0; i < n; ++i)
        this->data[i + offset] = data[i];
}


/*
 * CPU
 */

// Constructors 
CPUState::CPUState() : mem(0x10000)
{
    this->init();
}

CPUState::CPUState(unsigned int mem_size) : mem(mem_size)
{
    this->init();
}

void CPUState::init(void)
{
    this->pc = 0;
    this->sp = 0;
    this->ix = 0;
    this->iy = 0;

    for(int f = 0; f < 8; ++f)
        this->flags[f] = false;

    this->a = 0;
    this->b = 0;
    this->c = 0;
    this->d = 0;
    this->e = 0;
    this->h = 0;
    this->l = 0;
    this->w = 0;
    this->z = 0;

    this->data_bus = 0;
    this->adr_bus = 0;

    // internals 
    this->cyc_count = 0;
}

// operators 
bool CPUState::operator==(const CPUState& that) const
{
    if(this->pc != that.pc)
        return false;
    if(this->sp != that.sp)
        return false;
    if(this->ix != that.ix)
        return false;
    for(int f = 0; f < 8; ++f)
    {
        if(this->flags[f] != that.flags[f])
            return false;
    }
    // general registers
    if(this->a != that.a)
        return false;
    if(this->b != that.b)
        return false;
    if(this->c != that.c)
        return false;
    if(this->d != that.d)
        return false;
    if(this->e != that.e)
        return false;
    if(this->h != that.h)
        return false;
    if(this->l != that.l)
        return false;
    if(this->w != that.w)
        return false;
    if(this->z != that.z)
        return false;
    // NOTE: for now we don't compare the cycle count since that requires
    // each machine to have been running for the same amount of time

    return true;
}

bool CPUState::operator!=(const CPUState& that) const
{
    return !(*this == that);
}


// ======== LOGIC OPERANDS ======== //
void CPUState::op_and(const uint8_t val)
{
    uint8_t result = this->a & val;

    this->flags[FLAG_SIGN] = result >> 7;
    this->flags[FLAG_ZERO] = result == 0;
    this->flags[FLAG_PARITY] = parity(result);
    this->flags[FLAG_SUB] = false;
    this->flags[FLAG_CARRY] = false;
    this->flags[FLAG_F5] = (result & (1 << 5));
    this->flags[FLAG_F3] = (result & (1 << 3));

    this->a = result;
}

void CPUState::op_or(const uint8_t val)
{
    uint8_t result = this->a | val;

    this->flags[FLAG_SIGN] = result >> 7;
    this->flags[FLAG_ZERO] = result == 0;
    this->flags[FLAG_PARITY] = parity(result);
    this->flags[FLAG_SUB] = false;
    this->flags[FLAG_CARRY] = false;
    this->flags[FLAG_F5] = (result & (1 << 5));
    this->flags[FLAG_F3] = (result & (1 << 3));

    this->a = result;
}

void CPUState::op_xor(const uint8_t val)
{
    uint8_t result = this->a ^ val;

    this->flags[FLAG_SIGN] = result >> 7;
    this->flags[FLAG_ZERO] = result == 0;
    this->flags[FLAG_PARITY] = parity(result);
    this->flags[FLAG_SUB] = false;
    this->flags[FLAG_CARRY] = false;
    this->flags[FLAG_F5] = (result & (1 << 5));
    this->flags[FLAG_F3] = (result & (1 << 3));

    this->a = result;
}


// ======== MACHINE CYCLES ======== //
void CPUState::fetch(void)
{
    this->adr_bus = this->pc;
    this->pc++;                 
    this->data_bus = this->mem[this->adr_bus];
}

void CPUState::decode(void)
{
    this->dec_x = (this->data_bus & 0xC0) >> 6;
    this->dec_y = (this->data_bus & 0x38) >> 3;
    this->dec_z = (this->data_bus & 0x7);

    this->dec_p = (this->dec_y >> 1);
    this->dec_q = (this->dec_y % 2);
}

/*
 * exec_opcode()
 */
void CPUState::exec_opcode(void)
{
    switch(this->data_bus)
    {
        // basic loads 
        case 0x7F: this->a = this->a; break;    // LD A, A
        case 0x78: this->a = this->b; break;    // LD A, B
        case 0x79: this->a = this->c; break;    // LD A, C
        case 0x7A: this->a = this->d; break;    // LD A, D
        case 0x7B: this->a = this->e; break;    // LD A, E
        case 0x7C: this->a = this->h; break;    // LD A, H
        case 0x7D: this->a = this->l; break;    // LD A, L

        case 0x47: this->b = this->a; break;    // LD B, A
        case 0x40: this->b = this->b; break;    // LD B, B
        case 0x41: this->b = this->c; break;    // LD B, C
        case 0x42: this->b = this->d; break;    // LD B, D
        case 0x43: this->b = this->e; break;    // LD B, E
        case 0x44: this->b = this->h; break;    // LD B, H
        case 0x45: this->b = this->l; break;    // LD B, L

        case 0x4F: this->c = this->a; break;    // LD C, A
        case 0x48: this->c = this->b; break;    // LD C, B
        case 0x49: this->c = this->c; break;    // LD C, C
        case 0x4A: this->c = this->d; break;    // LD C, D
        case 0x4B: this->c = this->e; break;    // LD C, E
        case 0x4C: this->c = this->h; break;    // LD C, H
        case 0x4D: this->c = this->l; break;    // LD C, L

        case 0x57: this->d = this->a; break;    // LD D, A
        case 0x50: this->d = this->b; break;    // LD D, B
        case 0x51: this->d = this->c; break;    // LD D, C
        case 0x52: this->d = this->d; break;    // LD D, D
        case 0x53: this->d = this->e; break;    // LD D, E
        case 0x54: this->d = this->h; break;    // LD D, H
        case 0x55: this->d = this->l; break;    // LD D, L

        case 0x5F: this->e = this->a; break;    // LD E, A
        case 0x58: this->e = this->b; break;    // LD E, B
        case 0x59: this->e = this->c; break;    // LD E, C
        case 0x5A: this->e = this->d; break;    // LD E, D
        case 0x5B: this->e = this->e; break;    // LD E, E
        case 0x5C: this->e = this->h; break;    // LD E, H
        case 0x5D: this->e = this->l; break;    // LD E, L

        case 0x67: this->h = this->a; break;    // LD H, A
        case 0x60: this->h = this->b; break;    // LD H, B
        case 0x61: this->h = this->c; break;    // LD H, C
        case 0x62: this->h = this->d; break;    // LD H, D
        case 0x63: this->h = this->e; break;    // LD H, E
        case 0x64: this->h = this->h; break;    // LD H, H
        case 0x65: this->h = this->l; break;    // LD H, L

        case 0x6F: this->l = this->a; break;    // LD L, A
        case 0x68: this->l = this->b; break;    // LD L, B
        case 0x69: this->l = this->c; break;    // LD L, C
        case 0x6A: this->l = this->d; break;    // LD L, D
        case 0x6B: this->l = this->e; break;    // LD L, E
        case 0x6C: this->l = this->h; break;    // LD L, H
        case 0x6D: this->l = this->l; break;    // LD L, L

        // logic
        case 0xA7: this->op_and(this->a); break; // AND A
        case 0xA0: this->op_and(this->b); break; // AND B
        case 0xA1: this->op_and(this->c); break; // AND C
        case 0xA2: this->op_and(this->d); break; // AND D
        case 0xA3: this->op_and(this->e); break; // AND E
        case 0xA4: this->op_and(this->h); break; // AND H
        case 0xA5: this->op_and(this->l); break; // AND L
        case 0xA6: this->op_and(this->mem[this->read_hl()]); break;  // AND (HL)
        case 0xE7: this->op_and(this->mem[this->pc]); break;         // AND *

        case 0xB7: this->op_or(this->a); break; // AND A
        case 0xB0: this->op_or(this->b); break; // AND B
        case 0xB1: this->op_or(this->c); break; // AND C
        case 0xB2: this->op_or(this->d); break; // AND D
        case 0xB3: this->op_or(this->e); break; // AND E
        case 0xB4: this->op_or(this->h); break; // AND H
        case 0xB5: this->op_or(this->l); break; // AND L
        case 0xB6: this->op_or(this->mem[this->read_hl()]); break;  // AND (HL)
        case 0xF6: this->op_or(this->mem[this->pc]); break;         // AND *

        case 0xAF: this->op_xor(this->a); break;    // XOR A
        case 0xA8: this->op_xor(this->b); break;    // XOR B
        case 0xA9: this->op_xor(this->c); break;    // XOR C
        case 0xAA: this->op_xor(this->d); break;    // XOR D
        case 0xAB: this->op_xor(this->e); break;    // XOR E
        case 0xAC: this->op_xor(this->h); break;    // XOR H
        case 0xAD: this->op_xor(this->l); break;    // XOR L
        case 0xAE: this->op_xor(this->mem[this->read_hl()]); break;    // XOR (HL)
        case 0xEE: this->op_xor(this->mem[this->pc]); break;        // XOR *

        // CB prefixed instructions
        case 0xCB:
            this->exec_cb_opcode();
            break;

        default:
            std::cerr << "[" << __func__ << "] unknown opcode 0x" << std::hex << unsigned(this->data_bus)
                << std::endl;
    }

}

void CPUState::exec_cb_opcode(void)
{
    // decode instruction
    switch(this->dec_z)
    {
        case 0:     // relative jumps and some misc ops 
            break;
        case 2:     // indirect loading
            std::cout << "[" << __func__ << "] TODO: indirect loading " << std::endl;
            break;
        case 3:     // 16 bit increment/decrement
            break;
        case 4:     // 8-bit increment
            break;
        case 5:     // 8-bit decrement
            break;
        case 6:     // LD r[y], r[z]
            break;


        default:
            std::cout << "[" << __func__ << "] invalid code sequence " << std::endl;
            std::cout << "x: " << unsigned(this->dec_x) 
                << ", y: " << unsigned(this->dec_y) 
                << ", z: " << unsigned(this->dec_z) 
                << ", p: " << unsigned(this->dec_p)
                << ", q: " << unsigned(this->dec_q) 
                << std::endl;
            break;
    } 
}

/*
 * cycle()
 */
void CPUState::cycle(void)
{
    this->fetch();
    this->decode();     // NOTE: we don't need to do this each time, but we will for now
    this->exec_opcode();
}

// 16-bit reads and writes 

uint16_t CPUState::read_bc(void)
{
    return (this->b << 8) | this->c;
}
uint16_t CPUState::read_de(void)
{
    return (this->d << 8) | this->e;
}
uint16_t CPUState::read_hl(void)
{
    return (this->h << 8) | this->l;
}

void CPUState::write_bc(const uint16_t val)
{
    this->b = val >> 8;
    this->c = val & 0xFF;
}
void CPUState::write_de(const uint16_t val)
{
    this->d = val >> 8;
    this->e = val & 0xFF;
}
void CPUState::write_hl(const uint16_t val)
{
    this->h = val >> 8;
    this->l = val & 0xFF;
}
