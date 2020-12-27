/*
 * CPU
 * Components in the Z80 CPU
 */


#include "CPU.hpp"

// Constructors 
CPUState::CPUState() : pc(0), sp(0), ix(0), iy(0), 
    acc(0), flags(0),
    bc(0), de(0), hl(0), wz(0) {}



// operators 


// read helpers
uint8_t CPUState::read_b(void) const
{
    return (this->bc & 0xFF00) >> 8;
}

uint8_t CPUState::read_c(void) const
{
    return (this->bc & 0x00FF);
}

uint8_t CPUState::read_d(void) const
{
    return (this->de & 0xFF00) >> 8;
}

uint8_t CPUState::read_e(void) const
{
    return (this->de & 0x00FF);
}


uint8_t CPUState::read_h(void) const
{
    return (this->hl & 0xFF00) >> 8;
}

uint8_t CPUState::read_l(void) const
{
    return (this->hl & 0x00FF);
}

uint8_t CPUState::read_w(void) const
{
    return (this->wz & 0xFF00) >> 8;
}

uint8_t CPUState::read_z(void) const
{
    return (this->wz & 0x00FF);
}


uint16_t CPUState::read_bc(void) const
{
    return this->bc;
}

// write helpers
void CPUState::write_b(uint8_t v)
{
    this->bc = ((this->bc & 0x00FF) | (v << 8));
}
void CPUState::write_c(uint8_t v)
{
    this->bc = ((this->bc & 0xFF00) | v);
}


void CPUState::write_bc(uint16_t v)
{
    this->bc = v;
}



