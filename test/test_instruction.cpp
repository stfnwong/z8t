/*
 * TEST_INSTRUCTION
 */

#define CATCH_CONFIG_MAIN
#include "catch/catch.hpp"

#include <iostream> 
#include <iomanip>
#include <vector>
#include <string>

#include "Instruction.hpp"


TEST_CASE("test_all_instructions", "[sanity]")
{
    const std::vector<TextLine> input_instrs = {
        // adds 
        TextLine(Token(SYM_INSTR, INSTR_ADD, "add"), Token(SYM_REG, REG_A, "a"),   Token(SYM_REG, REG_A, "a")),
        TextLine(Token(SYM_INSTR, INSTR_ADD, "add"), Token(SYM_REG, REG_A, "a"),   Token(SYM_REG, REG_B, "b")),
        TextLine(Token(SYM_INSTR, INSTR_ADD, "add"), Token(SYM_REG, REG_A, "a"),   Token(SYM_REG, REG_C, "c")),
        TextLine(Token(SYM_INSTR, INSTR_ADD, "add"), Token(SYM_REG, REG_A, "a"),   Token(SYM_REG, REG_D, "d")),
        TextLine(Token(SYM_INSTR, INSTR_ADD, "add"), Token(SYM_REG, REG_A, "a"),   Token(SYM_REG, REG_E, "e")),
        TextLine(Token(SYM_INSTR, INSTR_ADD, "add"), Token(SYM_REG, REG_A, "a"),   Token(SYM_REG, REG_H, "h")),
        TextLine(Token(SYM_INSTR, INSTR_ADD, "add"), Token(SYM_REG, REG_A, "a"),   Token(SYM_REG, REG_L, "l")),
        TextLine(Token(SYM_INSTR, INSTR_ADD, "add"), Token(SYM_REG, REG_A, "a"),   Token(SYM_REG, REG_HL_IND, "(hl)")),
        TextLine(Token(SYM_INSTR, INSTR_ADD, "add"), Token(SYM_REG, REG_A, "a"),   Token(SYM_LITERAL, 128, "128")),
        TextLine(Token(SYM_INSTR, INSTR_ADD, "add"), Token(SYM_REG, REG_HL, "hl"), Token(SYM_REG, REG_BC, "bc")),
        TextLine(Token(SYM_INSTR, INSTR_ADD, "add"), Token(SYM_REG, REG_HL, "hl"), Token(SYM_REG, REG_DE, "de")),
        TextLine(Token(SYM_INSTR, INSTR_ADD, "add"), Token(SYM_REG, REG_HL, "hl"), Token(SYM_REG, REG_HL, "a")),
        TextLine(Token(SYM_INSTR, INSTR_ADD, "add"), Token(SYM_REG, REG_HL, "hl"), Token(SYM_REG, REG_SP, "sp")),
        // adc
        TextLine(Token(SYM_INSTR, INSTR_ADC, "adc"), Token(SYM_REG, REG_A, "a"),   Token(SYM_REG, REG_A, "a")),
        TextLine(Token(SYM_INSTR, INSTR_ADC, "adc"), Token(SYM_REG, REG_A, "a"),   Token(SYM_REG, REG_B, "b")),
        TextLine(Token(SYM_INSTR, INSTR_ADC, "adc"), Token(SYM_REG, REG_A, "a"),   Token(SYM_REG, REG_C, "c")),
        TextLine(Token(SYM_INSTR, INSTR_ADC, "adc"), Token(SYM_REG, REG_A, "a"),   Token(SYM_REG, REG_D, "d")),
        TextLine(Token(SYM_INSTR, INSTR_ADC, "adc"), Token(SYM_REG, REG_A, "a"),   Token(SYM_REG, REG_E, "e")),
        TextLine(Token(SYM_INSTR, INSTR_ADC, "adc"), Token(SYM_REG, REG_A, "a"),   Token(SYM_REG, REG_H, "h")),
        TextLine(Token(SYM_INSTR, INSTR_ADC, "adc"), Token(SYM_REG, REG_A, "a"),   Token(SYM_REG, REG_L, "l")),
        TextLine(Token(SYM_INSTR, INSTR_ADC, "adc"), Token(SYM_REG, REG_A, "a"),   Token(SYM_REG, REG_HL_IND, "(hl)")),
        TextLine(Token(SYM_INSTR, INSTR_ADC, "adc"), Token(SYM_REG, REG_A, "a"),   Token(SYM_LITERAL, 128, "128")),
        // and
        TextLine(Token(SYM_INSTR, INSTR_AND, "and"), Token(SYM_REG, REG_A, "a"),   Token()),
        TextLine(Token(SYM_INSTR, INSTR_AND, "and"), Token(SYM_REG, REG_A, "a"),   Token()),
        TextLine(Token(SYM_INSTR, INSTR_AND, "and"), Token(SYM_REG, REG_A, "a"),   Token()),
        TextLine(Token(SYM_INSTR, INSTR_AND, "and"), Token(SYM_REG, REG_A, "a"),   Token()),
        TextLine(Token(SYM_INSTR, INSTR_AND, "and"), Token(SYM_REG, REG_A, "a"),   Token()),
        TextLine(Token(SYM_INSTR, INSTR_AND, "and"), Token(SYM_REG, REG_A, "a"),   Token()),
        TextLine(Token(SYM_INSTR, INSTR_AND, "and"), Token(SYM_REG, REG_A, "a"),   Token()),
        TextLine(Token(SYM_INSTR, INSTR_AND, "and"), Token(SYM_REG, REG_A, "a"),   Token()),
        TextLine(Token(SYM_INSTR, INSTR_AND, "and"), Token(SYM_REG, REG_A, "a"),   Token()),
        // cp
        TextLine(Token(SYM_INSTR, INSTR_CP, "cp"), Token(SYM_REG, REG_A, "a"),     Token()),
        TextLine(Token(SYM_INSTR, INSTR_CP, "cp"), Token(SYM_REG, REG_A, "a"),     Token()),
        TextLine(Token(SYM_INSTR, INSTR_CP, "cp"), Token(SYM_REG, REG_A, "a"),     Token()),
        TextLine(Token(SYM_INSTR, INSTR_CP, "cp"), Token(SYM_REG, REG_A, "a"),     Token()),
        TextLine(Token(SYM_INSTR, INSTR_CP, "cp"), Token(SYM_REG, REG_A, "a"),     Token()),
        TextLine(Token(SYM_INSTR, INSTR_CP, "cp"), Token(SYM_REG, REG_A, "a"),     Token()),
        TextLine(Token(SYM_INSTR, INSTR_CP, "cp"), Token(SYM_REG, REG_A, "a"),     Token()),
        TextLine(Token(SYM_INSTR, INSTR_CP, "cp"), Token(SYM_REG, REG_A, "a"),     Token()),
        TextLine(Token(SYM_INSTR, INSTR_CP, "cp"), Token(SYM_REG, REG_A, "a"),     Token()),
        // inc
        TextLine(Token(SYM_INSTR, INSTR_INC, "inc"), Token(SYM_REG, REG_A, "a"),   Token()),
        TextLine(Token(SYM_INSTR, INSTR_INC, "inc"), Token(SYM_REG, REG_A, "a"),   Token()),
        TextLine(Token(SYM_INSTR, INSTR_INC, "inc"), Token(SYM_REG, REG_A, "a"),   Token()),
        TextLine(Token(SYM_INSTR, INSTR_INC, "inc"), Token(SYM_REG, REG_A, "a"),   Token()),
        TextLine(Token(SYM_INSTR, INSTR_INC, "inc"), Token(SYM_REG, REG_A, "a"),   Token()),
        TextLine(Token(SYM_INSTR, INSTR_INC, "inc"), Token(SYM_REG, REG_A, "a"),   Token()),
        TextLine(Token(SYM_INSTR, INSTR_INC, "inc"), Token(SYM_REG, REG_A, "a"),   Token()),
        TextLine(Token(SYM_INSTR, INSTR_INC, "inc"), Token(SYM_REG, REG_A, "a"),   Token()),
        TextLine(Token(SYM_INSTR, INSTR_INC, "inc"), Token(SYM_REG, REG_A, "a"),   Token()),
        // jr 
        TextLine(Token(SYM_INSTR, INSTR_JR, "jr"), Token(SYM_LITERAL, 64,   "64"), Token()),
        TextLine(Token(SYM_INSTR, INSTR_JR, "jr"), Token(SYM_COND, COND_Z,  "z"),  Token(SYM_LITERAL, 32, "32")),
        TextLine(Token(SYM_INSTR, INSTR_JR, "jr"), Token(SYM_COND, COND_C,  "c"),  Token(SYM_LITERAL, 32, "32")),
        TextLine(Token(SYM_INSTR, INSTR_JR, "jr"), Token(SYM_COND, COND_NZ, "nz"), Token(SYM_LITERAL, 32, "32")),
        TextLine(Token(SYM_INSTR, INSTR_JR, "jr"), Token(SYM_COND, COND_NC, "nc"), Token(SYM_LITERAL, 32, "32")),
        // jp
        TextLine(Token(SYM_INSTR, INSTR_JP, "jp"), Token(SYM_COND, COND_NZ, "nz"), Token(SYM_LITERAL, 32, "32")),
        TextLine(Token(SYM_INSTR, INSTR_JP, "jp"), Token(SYM_COND, COND_NC, "nc"), Token(SYM_LITERAL, 32, "32")),
        TextLine(Token(SYM_INSTR, INSTR_JP, "jp"), Token(SYM_COND, COND_PO, "po"), Token(SYM_LITERAL, 32, "32")),
        TextLine(Token(SYM_INSTR, INSTR_JP, "jp"), Token(SYM_COND, COND_P,  "p"),  Token(SYM_LITERAL, 32, "32")),
        TextLine(Token(SYM_INSTR, INSTR_JP, "jp"), Token(SYM_LITERAL, 196,  "196"), Token()),
        TextLine(Token(SYM_INSTR, INSTR_JP, "jp"), Token(SYM_REG, REG_HL_IND, "(hl)"), Token()),
        TextLine(Token(SYM_INSTR, INSTR_JP, "jp"), Token(SYM_COND, COND_Z,  "z"),  Token(SYM_LITERAL, 32, "32")),
        TextLine(Token(SYM_INSTR, INSTR_JP, "jp"), Token(SYM_COND, COND_C,  "c"),  Token(SYM_LITERAL, 32, "32")),
        TextLine(Token(SYM_INSTR, INSTR_JP, "jp"), Token(SYM_COND, COND_PE, "pe"), Token(SYM_LITERAL, 32, "32")),
        TextLine(Token(SYM_INSTR, INSTR_JP, "jp"), Token(SYM_COND, COND_M,  "m"),  Token(SYM_LITERAL, 32, "32")),
        // loads (actually not worth doing all the loads, just the more awkward/complex ones)
        TextLine(Token(SYM_INSTR, INSTR_LD, "ld"), Token(SYM_REG, REG_A, "a"), Token(SYM_REG, REG_A, "a")),
        TextLine(Token(SYM_INSTR, INSTR_LD, "ld"), Token(SYM_REG, REG_A, "a"), Token(SYM_REG, REG_A, "b")),
        TextLine(Token(SYM_INSTR, INSTR_LD, "ld"), Token(SYM_REG, REG_A, "a"), Token(SYM_REG, REG_A, "c")),
        TextLine(Token(SYM_INSTR, INSTR_LD, "ld"), Token(SYM_REG, REG_A, "a"), Token(SYM_REG, REG_A, "d")),
        TextLine(Token(SYM_INSTR, INSTR_LD, "ld"), Token(SYM_REG, REG_A, "a"), Token(SYM_REG, REG_A, "e")),
        TextLine(Token(SYM_INSTR, INSTR_LD, "ld"), Token(SYM_REG, REG_A, "a"), Token(SYM_REG, REG_A, "h")),
        TextLine(Token(SYM_INSTR, INSTR_LD, "ld"), Token(SYM_REG, REG_A, "a"), Token(SYM_REG, REG_A, "l")),
        // load immediates 
        TextLine(Token(SYM_INSTR, INSTR_LD, "ld"), Token(SYM_REG, REG_BC, "bc"), Token(SYM_LITERAL, 200, "200")),
        TextLine(Token(SYM_INSTR, INSTR_LD, "ld"), Token(SYM_REG, REG_DE, "de"), Token(SYM_LITERAL, 200, "200")),
        TextLine(Token(SYM_INSTR, INSTR_LD, "ld"), Token(SYM_REG, REG_SP, "sp"), Token(SYM_LITERAL, 200, "200")),
        // 
        TextLine(Token(SYM_INSTR, INSTR_LD, "ld"), Token(SYM_LITERAL_IND, 8, "(8)"), Token(SYM_REG, REG_HL, "hl")),
        TextLine(Token(SYM_INSTR, INSTR_LD, "ld"), Token(SYM_REG, REG_H, "h"), Token(SYM_REG, REG_HL_IND, "(hl)")),
    };

    std::cout << std::dec << input_instrs.size() << " instructions in test" << std::endl;

    for(unsigned int idx = 0; idx < input_instrs.size(); ++idx)
    {
        uint32_t line_hash = input_instrs[idx].argHash();
        auto lookup_val = instr_hash_to_code.find(line_hash);
        if(lookup_val == instr_hash_to_code.end())
        {
            std::cout << "Error in instruction [" << std::dec << idx << "/" << std::dec << input_instrs.size() << "] : " 
                << input_instrs[idx].toInstrString() << std::endl;
        }
        REQUIRE(lookup_val != instr_hash_to_code.end());
    }
}
