/*
 * CPU
 * Components in the Z80 CPU
 */

#ifndef __CPU_HPP
#define __CPU_HPP

#include <cstdint>

static inline bool parity(const uint8_t val)
{
    uint8_t num_set_bits = 0;       // set value is 1 
    for(int b = 0; b < 8; ++b)
        num_set_bits += ((val >> b) & 0x1);

    return (num_set_bits & 1) == 0;
}

/*
 * A block of memory attached to a CPU
 */
class Memory
{
    unsigned int mem_size;
    uint8_t*     data;

    public:
        Memory(unsigned int s);
        Memory(const Memory& that);
        ~Memory();

        uint8_t&      operator[](unsigned int i);
        const uint8_t operator[](unsigned int i) const;
        bool          operator==(const Memory& that) const;
        bool          operator!=(const Memory& that) const;

        void         clear(void);
        unsigned int size(void) const;

        void         load(const uint8_t* data, unsigned int n, unsigned int offset);
        //void         load(const std::vector<uint8_t>& data, unsigned int size, unsigned int offset);
};

enum {FLAG_CARRY = 0, FLAG_SUB, FLAG_PARITY, FLAG_F3, FLAG_HALF, FLAG_F5, FLAG_ZERO, FLAG_SIGN };

/*
 * CPUState
 * Holds registers, program counter, stack pointer
 */
struct CPUState
{
    Memory   mem;
    // 16-bit registers
    uint16_t pc;
    uint16_t sp;
    uint16_t ix;
    uint16_t iy;

    // flags register
    uint8_t flags;

    // general registers 
    // TODO: shadow registers
    uint8_t a;
    uint8_t b;
    uint8_t c;
    uint8_t d;
    uint8_t e;
    uint8_t h;
    uint8_t l;
    uint8_t w;
    uint8_t z;

    // Other internal registers (not tested for equality)
    uint16_t adr_bus;
    uint16_t data_bus;
    // TODO: instruction register?

    // decode internals
    uint8_t dec_x, dec_y, dec_z; 
    uint8_t dec_p, dec_q;

    // other timing internals 
    unsigned int cyc_count;

    public:
        CPUState();
        CPUState(unsigned int mem_size);
        //CPUState(const CPUState& that) = default;
        void init(void);

        bool operator==(const CPUState& that) const;
        bool operator!=(const CPUState& that) const;

        // logic operations
        void op_and(const uint8_t val);
        void op_or(const uint8_t val);
        void op_xor(const uint8_t val);

        // Machine cycles 
        void fetch(void);
        void decode(void);
        void exec_opcode(void);
        void exec_cb_opcode(void);
        //void exec_ed_opcode(void);
        //void exec_dd_opcode(void);
        //void exec_fd_opcode(void);
        //void exec_ddcb_opcode(void);
        //void exec_fdcb_opcode(void);
        void cycle(void);

        // 16-bit reads and writes
        uint16_t read_bc(void);
        uint16_t read_de(void);
        uint16_t read_hl(void);

        void write_bc(const uint16_t val);
        void write_de(const uint16_t val);
        void write_hl(const uint16_t val);

        // flags (these should become private later..
        void set_flag(const uint8_t flag);
        void clear_flag(const uint8_t flag);
        bool get_flag(const uint8_t flag);
};



#endif /*__CPU_HPP*/
