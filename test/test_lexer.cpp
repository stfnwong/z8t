/* TEST_LEXER 
 * Tests for Z80 psuedo-assembler 
 *
 * Stefan Wong 2018
 */

#include <iostream> 
#include <iomanip>
#include <vector>
#include <string>
#include <gtest/gtest.h>
#include "lexer.hpp"
#include "source.hpp"


class TestLexer : public ::testing::Test
{
    virtual void SetUp() {}
    virtual void TearDown() {}
};

TEST_F(TestLexer, test_inc_asm)
{
    int status;
    std::string infile = "data/add_sub.asm";
    Lexer lexer;
    lexer.setVerbose(true);
    SourceInfo lex_source;

    std::cout << "\t Reading file " << infile << std::endl;
    status = lexer.read(infile);
    ASSERT_EQ(0, status);

    std::cout << "\t Lexing file " << infile << std::endl;
    lexer.lex();

    // Get the source 
    lex_source = lexer.getSource();
    std::cout << "\t Lexer generated " << lex_source.getNumLines() << " line of output" << std::endl;

    std::cout << "Dumping source info...." << std::endl << std::endl;
    for(unsigned int line = 0; line < lex_source.getNumLines(); ++line)
    {
        LineInfo line_info = lex_source.get(line);
        std::cout << line_info.toString() << std::endl;
    }
    
    std::cout << std::endl << "... done" << std::endl;
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

