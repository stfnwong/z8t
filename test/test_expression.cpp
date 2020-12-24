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


TEST_CASE("test_expr_next_token_simple", "[classic]")
{
    // the sequence of tokens emitted by repeat calls should look like
    std::vector<ParseResult> exp_pairs = {
        ParseResult(ExprToken(TOK_LITERAL,     0, "1"), 2),
        ParseResult(ExprToken(TOK_SLASH,       0, "/"), 3),
        ParseResult(ExprToken(TOK_LEFT_PAREN,  0, "("), 5),
        ParseResult(ExprToken(TOK_LITERAL,     0, "2"), 7), 
        ParseResult(ExprToken(TOK_STAR,        0, "*"), 8),
        ParseResult(ExprToken(TOK_LITERAL,     0, "3"), 10),
        ParseResult(ExprToken(TOK_RIGHT_PAREN, 0, ")"), 11)
    };
    const std::string expr_input = "1 / (2 * 3)";

    ParseResult parse_result;
    for(unsigned int idx = 0; idx < exp_pairs.size(); ++idx)
    {
        if(idx == 0)
            parse_result = expr_next_token(expr_input, 0);
        else
            parse_result = expr_next_token(expr_input, parse_result.pos);

        REQUIRE(parse_result == exp_pairs[idx]);
    }
}

TEST_CASE("test_expr_next_token_negative", "[classic]")
{
    std::vector<ParseResult> exp_pairs = {
        ParseResult(ExprToken(TOK_MINUS,       0, "-"), 1),
        ParseResult(ExprToken(TOK_LITERAL,     0, "1"), 3),
        ParseResult(ExprToken(TOK_SLASH,       0, "/"), 4),
        ParseResult(ExprToken(TOK_LEFT_PAREN,  0, "("), 6),
        ParseResult(ExprToken(TOK_MINUS,       0, "-"), 7),
        ParseResult(ExprToken(TOK_LITERAL,     0, "2"), 9),       // 9 because we consume one space
        ParseResult(ExprToken(TOK_STAR,        0, "*"), 10),
        ParseResult(ExprToken(TOK_MINUS,       0, "-"), 12),
        ParseResult(ExprToken(TOK_LITERAL,     0, "3"), 13),
        ParseResult(ExprToken(TOK_RIGHT_PAREN, 0, ")"), 14)
    };
    const std::string expr_input = "-1 / (-2 * -3)";

    ParseResult parse_result;
    for(unsigned int idx = 0; idx < exp_pairs.size(); ++idx)
    {
        if(idx == 0)
            parse_result = expr_next_token(expr_input, 0);
        else
            parse_result = expr_next_token(expr_input, parse_result.pos);

        REQUIRE(parse_result == exp_pairs[idx]);
    }
}

TEST_CASE("test_eval_expr_string", "[classic]")
{
    //const std::string expr_input = "-1 / (-2 * -3)";
    //Expression exp_expr(expr_input, -1 / (-2 * -3));
    const std::string expr_input = "3 + 4 * 2 / (1 - 5)";
    Expression exp_expr(expr_input, 3 + 4 * 2 / (1 - 5));

    std::cout << "Checking expr_string [" << expr_input << "]" << std::endl;

    Expression out_expr = eval_expr_string(expr_input);

    std::cout << "output : " << out_expr.toString() << std::endl;
}
