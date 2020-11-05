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


TEST_CASE("test_next_token_simple", "[classic]")
{
    // the sequence of tokens emitted by repeat calls should look like
    std::vector<std::pair<ExprToken, int>> exp_pairs = {
        std::pair<ExprToken, int>(ExprToken(TOK_LITERAL,     0, "1"), 2),
        std::pair<ExprToken, int>(ExprToken(TOK_SLASH,       0, "/"), 3),
        std::pair<ExprToken, int>(ExprToken(TOK_LEFT_PAREN,  0, "("), 5),
        std::pair<ExprToken, int>(ExprToken(TOK_LITERAL,     0, "2"), 7), 
        std::pair<ExprToken, int>(ExprToken(TOK_STAR,        0, "*"), 8),
        std::pair<ExprToken, int>(ExprToken(TOK_LITERAL,     0, "3"), 10),
        std::pair<ExprToken, int>(ExprToken(TOK_RIGHT_PAREN, 0, ")"), 11)
    };
    const std::string expr_input = "1 / (2 * 3)";

    std::cout << "Expecting " << std::dec << exp_pairs.size() << " pairs" << std::endl;

    std::pair<ExprToken, int> out_pair;
    for(unsigned int idx = 0; idx < exp_pairs.size(); ++idx)
    {
        if(idx == 0)
            out_pair = next_token(expr_input, 0);
        else
            out_pair = next_token(expr_input, out_pair.second);

        std::cout << "Pair " << std::dec << idx+1 << " : " << out_pair.first.toString()
            << ",  next index: " << std::to_string(out_pair.second) << std::endl;

        REQUIRE(out_pair == exp_pairs[idx]);
    }
}

TEST_CASE("test_next_token_negative", "[classic]")
{
    std::vector<std::pair<ExprToken, int>> exp_pairs = {
        std::pair<ExprToken, int>(ExprToken(TOK_MINUS,       0, "-"), 1),
        std::pair<ExprToken, int>(ExprToken(TOK_LITERAL,     0, "1"), 3),
        std::pair<ExprToken, int>(ExprToken(TOK_SLASH,       0, "/"), 4),
        std::pair<ExprToken, int>(ExprToken(TOK_LEFT_PAREN,  0, "("), 6),
        std::pair<ExprToken, int>(ExprToken(TOK_MINUS,       0, "-"), 7),
        std::pair<ExprToken, int>(ExprToken(TOK_LITERAL,     0, "2"), 9),       // 9 because we consume one space
        std::pair<ExprToken, int>(ExprToken(TOK_STAR,        0, "*"), 10),
        std::pair<ExprToken, int>(ExprToken(TOK_MINUS,       0, "-"), 12),
        std::pair<ExprToken, int>(ExprToken(TOK_LITERAL,     0, "3"), 13),
        std::pair<ExprToken, int>(ExprToken(TOK_RIGHT_PAREN, 0, ")"), 14)
    };
    const std::string expr_input = "-1 / (-2 * -3)";

    std::cout << "Expecting " << std::dec << exp_pairs.size() << " pairs" << std::endl;

    std::pair<ExprToken, int> out_pair;
    for(unsigned int idx = 0; idx < exp_pairs.size(); ++idx)
    {
        if(idx == 0)
            out_pair = next_token(expr_input, 0);
        else
            out_pair = next_token(expr_input, out_pair.second);

        std::cout << "Pair " << std::dec << idx+1 << " : " << out_pair.first.toString()
            << ",  next index: " << std::to_string(out_pair.second) << std::endl;

        REQUIRE(out_pair == exp_pairs[idx]);
    }
}
