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



TEST_CASE("test_lexer_init", "[classic]")
{
    int status;
    std::string infile = "data/add_sub.asm";
    Lexer lexer;
    lexer.setVerbose(true);
    SourceInfo lex_source;

    std::cout << "\t Reading file " << infile << std::endl;
    status = lexer.read(infile);
    REQUIRE(status == 0);

    std::cout << "\t Lexing file " << infile << std::endl;
    lexer.lex();

    // Get the source 
    lex_source = lexer.getSource();
    std::cout << "\t Lexer generated " << lex_source.getNumLines() << " line of output" << std::endl;

    std::cout << "Dumping source info...." << std::endl << std::endl;
    for(unsigned int line = 0; line < lex_source.getNumLines(); ++line)
    {
        TextLine line_info = lex_source.get(line);
        std::cout << line_info.toString() << std::endl;
    }
    
    std::cout << std::endl << "... done" << std::endl;
}
