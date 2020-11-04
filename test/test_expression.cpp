/*
 * TEST_EXPRESSION
 * Unit tests for expression evaluation routines
 *
 */


#define CATCH_CONFIG_MAIN
#include "catch/catch.hpp"

#include <iostream> 
#include <iomanip>
#include <string>
#include <utility>
#include <vector>

#include "Expression.hpp"


TEST_CASE("test_next_token", "[classic]")
{
    // the sequence of tokens emitted by repeat calls should look like
    std::vector<std::pair<Token, int>> exp_pairs = {
        std::pair<Token, int>(Token(SYM_NULL, -1, "1"), 2),
        std::pair<Token, int>(Token(SYM_NULL, -1, "/"), 3),
        std::pair<Token, int>(Token(SYM_NULL, -1, "("), 5),
        std::pair<Token, int>(Token(SYM_NULL, -1, "2"), 7), 
        std::pair<Token, int>(Token(SYM_NULL, -1, "*"), 8),
        std::pair<Token, int>(Token(SYM_NULL, -1, "3"), 10),
        std::pair<Token, int>(Token(SYM_NULL, -1, ")"), 11)
    };
    const std::string expr_input = "1 / (2 * 3)";

    std::cout << "Expecting " << std::dec << exp_pairs.size() << " pairs" << std::endl;
    std::pair<Token, int> out_pair;
    for(unsigned int idx = 0; idx < exp_pairs.size(); ++idx)
    {
        if(idx == 0)
            out_pair = next_token(expr_input, 0);
        else
            out_pair = next_token(expr_input, out_pair.second);

        std::cout << "Pair " << std::dec << idx+1 << " : " << out_pair.first.toString()
            << ",  " << std::to_string(out_pair.second) << std::endl;

        REQUIRE(out_pair == exp_pairs[idx]);
    }
}
