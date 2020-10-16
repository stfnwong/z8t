/* TEST_LEXER 
 * Tests for Z80 psuedo-assembler 
 *
 * Stefan Wong 2018
 */

#define CATCH_CONFIG_MAIN
#include "catch/catch.hpp"

#include <iostream> 
#include <iomanip>
#include <vector>
#include <string>

#include "Lexer.hpp"
#include "Source.hpp"


SourceInfo get_add_sub_expected_source(void)
{
    SourceInfo info;
    TextLine line;

    // ld a, 8
    line.init();
    line.line_num = 5;
    line.opcode = Opcode(INSTR_LD, "ld");
    line.addr = TEXT_START_ADDR;    
    line.args[0] = Argument(SYM_REG, REG_A, "a");
    line.args[1] = Argument(SYM_LITERAL, 8, "8");
    info.add(line);

    // add a, 10
    line.init();
    line.line_num = 6;
    line.opcode = Opcode(INSTR_ADD, "add");
    line.addr = TEXT_START_ADDR + 1;    
    line.args[0] = Argument(SYM_REG, REG_A, "a");
    line.args[1] = Argument(SYM_LITERAL, 10, "10");
    info.add(line);


    return info;
}


TEST_CASE("test_lexer_init", "[classic]")
{
    int status;
    std::string infile = "data/add_sub.asm";
    Lexer lexer;
    lexer.setVerbose(true);
    SourceInfo lex_source;
    SourceInfo exp_source;

    std::cout << "\t Reading file " << infile << std::endl;
    status = lexer.read(infile);
    REQUIRE(status == 0);

    std::cout << "\t Lexing file " << infile << std::endl;
    lexer.lex();

    // Get the source 
    lex_source = lexer.getSource();
    std::cout << "\t Lexer generated " << lex_source.getNumLines() << " line of output" << std::endl;


    exp_source = get_add_sub_expected_source();
    for(unsigned int i = 0; i < exp_source.getNumLines(); ++i)
    {
        TextLine exp_line = exp_source.get(i);
        TextLine lex_line = lex_source.get(i);

        if(exp_line != lex_line)
        {
            std::cout << "Error in line " << i+1 << "/" 
                << exp_source.getNumLines() << std::endl;

            std::cout << "Expected :" << std::endl;
            std::cout << exp_line.toString() << std::endl;

            std::cout << "Got :" << std::endl;
            std::cout << lex_line.toString() << std::endl;
        }

        REQUIRE(exp_line == lex_line);
    }
}
