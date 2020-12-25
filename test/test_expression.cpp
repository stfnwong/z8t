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


TEST_CASE("test_expr_next_token_simple", "[expression]")
{
    // the sequence of tokens emitted by repeat calls should look like
    std::vector<ParseResult> exp_pairs = {
        ParseResult(ExprToken(TOK_LITERAL, 0, "1"), 2),
        ParseResult(ExprToken(TOK_SLASH,   0, "/"), 3),
        ParseResult(ExprToken(TOK_LPAREN,  0, "("), 5),
        ParseResult(ExprToken(TOK_LITERAL, 0, "2"), 7), 
        ParseResult(ExprToken(TOK_STAR,    0, "*"), 8),
        ParseResult(ExprToken(TOK_LITERAL, 0, "3"), 10),
        ParseResult(ExprToken(TOK_RPAREN,  0, ")"), 11)
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

TEST_CASE("test_expr_next_token_negative", "[expression]")
{
    std::vector<ParseResult> exp_pairs = {
        ParseResult(ExprToken(TOK_MINUS,   0, "-"), 1),
        ParseResult(ExprToken(TOK_LITERAL, 0, "1"), 3),
        ParseResult(ExprToken(TOK_SLASH,   0, "/"), 4),
        ParseResult(ExprToken(TOK_LPAREN,  0, "("), 6),
        ParseResult(ExprToken(TOK_MINUS,   0, "-"), 7),
        ParseResult(ExprToken(TOK_LITERAL, 0, "2"), 9),       // 9 because we consume one space
        ParseResult(ExprToken(TOK_STAR,    0, "*"), 10),
        ParseResult(ExprToken(TOK_MINUS,   0, "-"), 12),
        ParseResult(ExprToken(TOK_LITERAL, 0, "3"), 13),
        ParseResult(ExprToken(TOK_RPAREN,  0, ")"), 14)
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


// TODO: this test is too minimal to be kept in the final version
TEST_CASE("test_expr_tokenize", "[expression]")
{
    const std::string expr_input = "3 + 4 * 2 / (1 - 5)";
    ExprStack infix_stack = expr_tokenize(expr_input);

    std::cout << "Input expression was [" << expr_input << "]" << std::endl;
    std::cout << infix_stack.size() << " elements in infix expression stack" << std::endl;
    for(unsigned int idx = 0; idx < infix_stack.size(); ++idx)
        std::cout << "[" << idx << "] : " << infix_stack[idx].toString() << std::endl;

    // Now try getting the equivalent infix stack
    ExprStack postfix_stack = expr_infix_to_postfix(infix_stack);
    std::cout << postfix_stack.size() << " elements in postfix expression stack" << std::endl;
    for(unsigned int idx = 0; idx < postfix_stack.size(); ++idx)
        std::cout << "[" << idx << "] : " << postfix_stack[idx].toString() << std::endl;

}

TEST_CASE("test_eval_expr_string", "[expression]")
{
    //const std::string expr_input = "-1 / (-2 * -3)";
    //Expression exp_expr(expr_input, -1 / (-2 * -3));
    const std::string expr_input = "3 + 4 * 2 / (1 - 5)";
    Expression exp_expr(expr_input, 3 + 4 * 2 / (1 - 5));

    std::cout << "Checking expr_string [" << expr_input << "]" << std::endl;

    Expression out_expr = eval_expr_string(expr_input);

    std::cout << "output : " << out_expr.toString() << std::endl;
}
