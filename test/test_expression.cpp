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
#include "Util.hpp"     // for equal()


TEST_CASE("test_expr_next_token_simple", "[expression]")
{
    // the sequence of tokens emitted by repeat calls should look like
    std::vector<ParseResult> exp_pairs = {
        ParseResult(ExprToken(TOK_LITERAL, "1"), 2),
        ParseResult(ExprToken(TOK_SLASH,   "/"), 3),
        ParseResult(ExprToken(TOK_LPAREN,  "("), 5),
        ParseResult(ExprToken(TOK_LITERAL, "2"), 7), 
        ParseResult(ExprToken(TOK_STAR,    "*"), 8),
        ParseResult(ExprToken(TOK_LITERAL, "3"), 10),
        ParseResult(ExprToken(TOK_RPAREN,  ")"), 11)
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
        ParseResult(ExprToken(TOK_MINUS,   "-"), 1),
        ParseResult(ExprToken(TOK_LITERAL, "1"), 3),
        ParseResult(ExprToken(TOK_SLASH,   "/"), 4),
        ParseResult(ExprToken(TOK_LPAREN,  "("), 6),
        ParseResult(ExprToken(TOK_MINUS,   "-"), 7),
        ParseResult(ExprToken(TOK_LITERAL, "2"), 9),       // 9 because we consume one space
        ParseResult(ExprToken(TOK_STAR,    "*"), 10),
        ParseResult(ExprToken(TOK_MINUS,   "-"), 12),
        ParseResult(ExprToken(TOK_LITERAL, "3"), 13),
        ParseResult(ExprToken(TOK_RPAREN,  ")"), 14)
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

TEST_CASE("test_expr_tokenize", "[expression]")
{
    const std::string expr_input = "3 + 4 * 2 / (1 - 5)";
    ExprStack infix_stack = expr_tokenize(expr_input);

    const std::vector<ExprToken> exp_infix_stack_vector = {
        ExprToken(TOK_LITERAL, "3"),
        ExprToken(TOK_PLUS,    "+"),
        ExprToken(TOK_LITERAL, "4"),
        ExprToken(TOK_STAR,    "*"),
        ExprToken(TOK_LITERAL, "2"),
        ExprToken(TOK_SLASH,   "/"),
        ExprToken(TOK_LPAREN,  "("),
        ExprToken(TOK_LITERAL, "1"),
        ExprToken(TOK_MINUS,   "-"),
        ExprToken(TOK_LITERAL, "5"),
        ExprToken(TOK_RPAREN, ")")
    };
    ExprStack exp_infix_stack(exp_infix_stack_vector);

    REQUIRE(exp_infix_stack == infix_stack);
}

TEST_CASE("test_expr_infix_to_postfix", "[expression]")
{
    const std::string expr_input = "3 + 4 * 2 / (1 - 5)";
    ExprStack infix_stack = expr_tokenize(expr_input);

    // Show tokenized infix expression
    std::cout << "Input expression was [" << expr_input << "]" << std::endl;
    std::cout << infix_stack.size() << " elements in infix expression stack" << std::endl;
    for(unsigned int idx = 0; idx < infix_stack.size(); ++idx)
        std::cout << "[" << idx << "] : " << infix_stack[idx].toString() << std::endl;

    const std::vector<ExprToken> exp_postfix_stack_vector = {
        ExprToken(TOK_LITERAL, "3"),
        ExprToken(TOK_LITERAL, "4"),
        ExprToken(TOK_LITERAL, "2"),
        ExprToken(TOK_STAR,    "*"),
        ExprToken(TOK_LITERAL, "1"),
        ExprToken(TOK_LITERAL, "5"),
        ExprToken(TOK_MINUS,   "-"),
        ExprToken(TOK_SLASH,   "/"),
        ExprToken(TOK_PLUS,    "+"),
    };
    ExprStack exp_postfix_stack(exp_postfix_stack_vector);

    // Now try getting the equivalent postfix stack
    ExprStack postfix_stack = expr_infix_to_postfix(infix_stack);
    std::cout << postfix_stack.size() << " elements in postfix expression stack" << std::endl;
    for(unsigned int idx = 0; idx < postfix_stack.size(); ++idx)
        std::cout << "[" << idx << "] : " << postfix_stack[idx].toString() << std::endl;

    REQUIRE(exp_postfix_stack == postfix_stack);
}

TEST_CASE("test_expr_eval", "[expression]")
{
    const std::string expr_input = "3 + 4 * 2 / (1 - 5)";
    ExprStack infix_stack = expr_tokenize(expr_input);
    ExprStack postfix_stack = expr_infix_to_postfix(infix_stack);

    float eval_out = eval_postfix_expr_stack(postfix_stack);
    //std::cout << "Expression [" << expr_input << "] evaluates to : " << eval_out << std::endl;
    //std::cout << "Expression [" << expr_input << "] C++          : " << (3 + 4 * 2 / (1 - 5)) << std::endl;

    REQUIRE(equal(eval_out, 1.0));
}
