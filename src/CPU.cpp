/*
 * CPU
 * Components in the Z80 CPU
 */

#include <cstring>
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

    this->acc = 0;
    this->flags = 0;

    this->bc = 0;
    this->de = 0;
    this->hl = 0;
    this->wz = 0;

    this->data_bus = 0;
    this->adr_bus = 0;
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
    if(this->acc != that.acc)
        return false;
    if(this->flags != that.flags)
        return false;
    if(this->bc != that.bc)
        return false;
    if(this->de != that.de)
        return false;
    if(this->hl != that.hl)
        return false;
    if(this->wz != that.wz)
        return false;

    return true;
}

bool CPUState::operator!=(const CPUState& that) const
{
    return !(*this == that);
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
    switch(this->data_bus)
    {
    }
}

void CPUState::exec(void)
{

}



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
uint16_t CPUState::read_de(void) const
{
    return this->de;
}
uint16_t CPUState::read_hl(void) const
{
    return this->hl;
}
uint16_t CPUState::read_wz(void) const
{
    return this->wz;
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
void CPUState::write_d(uint8_t v)
{
    this->de = ((this->de & 0x00FF) | (v << 8));
}
void CPUState::write_e(uint8_t v)
{
    this->de = ((this->de & 0xFF00) | v);
}
void CPUState::write_h(uint8_t v)
{
    this->hl = ((this->hl & 0x00FF) | (v << 8));
}
void CPUState::write_l(uint8_t v)
{
    this->hl = ((this->hl & 0xFF00) | v);
}
void CPUState::write_w(uint8_t v)
{
    this->wz = ((this->wz & 0x00FF)  | (v << 8));
}
void CPUState::write_z(uint8_t v)
{
    this->wz = ((this->wz & 0xFF00) | v);
}
void CPUState::write_bc(uint16_t v)
{
    this->bc = v;
}
void CPUState::write_de(uint16_t v)
{
    this->de = v;
}
void CPUState::write_hl(uint16_t v)
{
    this->hl = v;
}
void CPUState::write_wz(uint16_t v)
{
    this->wz = v;
}
