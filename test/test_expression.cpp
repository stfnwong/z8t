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
#include "Eval.hpp"     // for EvalResult
#include "Util.hpp"     // for equal()

constexpr const bool GLOBAL_VERBOSE = false;


TEST_CASE("test_expr_next_token_simple", "expression")
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

TEST_CASE("test_expr_next_token_negative", "expression")
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

TEST_CASE("test_expr_tokenize", "expression")
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

TEST_CASE("test_expr_tokenize_with_string", "expression")
{
    const std::string expr_input = "3 + foo * 2 / (1 - bar )";
    ExprStack infix_stack = expr_tokenize(expr_input);

    const std::vector<ExprToken> exp_infix_stack_vector = {
        ExprToken(TOK_LITERAL, "3"),
        ExprToken(TOK_PLUS,    "+"),
        ExprToken(TOK_STRING,  "foo"),
        ExprToken(TOK_STAR,    "*"),
        ExprToken(TOK_LITERAL, "2"),
        ExprToken(TOK_SLASH,   "/"),
        ExprToken(TOK_LPAREN,  "("),
        ExprToken(TOK_LITERAL, "1"),
        ExprToken(TOK_MINUS,   "-"),
        ExprToken(TOK_STRING,  "bar"),
        ExprToken(TOK_RPAREN, ")")
    };
    ExprStack exp_infix_stack(exp_infix_stack_vector);

    REQUIRE(exp_infix_stack == infix_stack);
    
    if(GLOBAL_VERBOSE)
    {
        std::cout << "Before symbol replacement : " << std::endl;
        for(unsigned int idx = 0; idx < infix_stack.size(); ++idx)
            std::cout << "[" << idx << "] : " << infix_stack[idx].toString() << std::endl;
    }

    // quick test of re-writing the stack contents 
    for(unsigned int idx = 0; idx < infix_stack.size(); ++idx)
    {
        if(infix_stack[idx].type == TOK_STRING)
        {
            if(infix_stack[idx].repr == "foo")
                infix_stack[idx] = ExprToken(TOK_LITERAL, std::string("10"));
            else if(infix_stack[idx].repr == "bar")
                infix_stack[idx] = ExprToken(TOK_LITERAL, std::string("20"));
        }
    }

    if(GLOBAL_VERBOSE)
    {
        std::cout << "After symbol replacement : " << std::endl;
        for(unsigned int idx = 0; idx < infix_stack.size(); ++idx)
            std::cout << "[" << idx << "] : " << infix_stack[idx].toString() << std::endl;
    }
}

TEST_CASE("test_expr_infix_to_postfix", "expression")
{
    const std::string expr_input = "3 + 4 * 2 / (1 - 5)";
    ExprStack infix_stack = expr_tokenize(expr_input);

    if(GLOBAL_VERBOSE)
    {
        // Show tokenized infix expression
        std::cout << "Input expression was [" << expr_input << "]" << std::endl;
        std::cout << infix_stack.size() << " elements in infix expression stack" << std::endl;
        for(unsigned int idx = 0; idx < infix_stack.size(); ++idx)
            std::cout << "[" << idx << "] : " << infix_stack[idx].toString() << std::endl;
    }

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
    if(GLOBAL_VERBOSE)
    {
        std::cout << postfix_stack.size() << " elements in postfix expression stack" << std::endl;
        for(unsigned int idx = 0; idx < postfix_stack.size(); ++idx)
            std::cout << "[" << idx << "] : " << postfix_stack[idx].toString() << std::endl;
    }

    REQUIRE(exp_postfix_stack == postfix_stack);
}

TEST_CASE("test_expr_literal", "expression")
{
    SourceInfo dummy_info;
    const std::string expr_input = "5";

    ExprStack infix_stack = expr_tokenize(expr_input);
    ExprStack postfix_stack = expr_infix_to_postfix(infix_stack);

    EvalResult eval_out = eval_postfix_expr_stack(postfix_stack);
    // ensure it also works with the eval wrapper 
    EvalResult wrapper_out = eval_expr_string(expr_input, dummy_info);

    REQUIRE(eval_out.val == 5);
    REQUIRE(eval_out.ok == true);
    REQUIRE(wrapper_out.val == 5);
    REQUIRE(wrapper_out.ok == true);
}

TEST_CASE("test_expr_eval", "expression")
{
    const std::string expr_input = "3 + 4 * 2 / (1 - 5)";
    ExprStack infix_stack = expr_tokenize(expr_input);
    ExprStack postfix_stack = expr_infix_to_postfix(infix_stack);

    EvalResult eval_out = eval_postfix_expr_stack(postfix_stack);
    if(GLOBAL_VERBOSE)
    {
        std::cout << "Expression [" << expr_input << "] evaluates to : " << eval_out.val << std::endl;
        std::cout << "Expression [" << expr_input << "] expected     : " << (3 + 4 * 2 / (1 - 5)) << std::endl;
    }

    REQUIRE(eval_out.val == 1);
}

SourceInfo get_expr_eval_source(void)
{
    SourceInfo info;
    int16_t x_addr = 0x1000;
    int16_t y_addr = 0x2000;

    // create symbols and directive to resolve against
    Symbol sym_x = Symbol(x_addr, "x");
    Symbol sym_y = Symbol(y_addr, "y");
    LineInfo dir_x;
    LineInfo dir_y;

    dir_x.type = LineType::DirectiveLine;
    dir_x.opcode = Token(SYM_DIRECTIVE, DIR_DEFW, ".defw");
    dir_x.addr = x_addr;
    dir_x.expr.push_back("5");

    dir_y.type = LineType::DirectiveLine;
    dir_y.opcode = Token(SYM_DIRECTIVE, DIR_DEFW, ".defw");
    dir_y.addr = y_addr;
    dir_y.expr.push_back("10");

    info.addSym(sym_x);
    info.addSym(sym_y);
    info.add(dir_x);
    info.add(dir_y);

    return info;
}


// Test that manually calling each method works
TEST_CASE("test_expr_eval_symbols", "expression")
{
    SourceInfo info;
    ExprStack token_stack;
    const std::string expr_input = "3 + x * y / (y - 5)";
    int exp_eval = int(3 + 5 * 10 / (10 - 5));

    info = get_expr_eval_source();

    token_stack = expr_tokenize(expr_input);
    if(GLOBAL_VERBOSE)
    {
        std::cout << "infix : " << std::endl;
        std::cout << token_stack.toString() << std::endl;
    }
    REQUIRE(token_stack[2].type == TOK_STRING);
    REQUIRE(token_stack[4].type == TOK_STRING);
    REQUIRE(token_stack[7].type == TOK_STRING);

    // NOTE: need to resolve before postfix conversion...
    std::pair<ExprStack,bool> resolved_stack = expr_stack_resolve_strings(token_stack, info);
    REQUIRE(resolved_stack.second == true);

    if(GLOBAL_VERBOSE)
    {
        std::cout << "resolved : " << std::endl;
        std::cout << resolved_stack.first.toString() << std::endl;
    }

    token_stack = expr_infix_to_postfix(resolved_stack.first);

    if(GLOBAL_VERBOSE)
    {
        std::cout << "postfix : " << std::endl;
        std::cout << token_stack.toString() << std::endl;
    }

    EvalResult eval_out = eval_postfix_expr_stack(token_stack);
    std::cout << "eval_out : " << eval_out.toString() << std::endl;
    REQUIRE(eval_out.val == exp_eval);
    REQUIRE(eval_out.ok == true);
}

TEST_CASE("test_expr_eval_symbols_direct", "directive")
{
    SourceInfo info;
    const std::string expr_input = "3 + x * y / (y - 5)";
    int exp_eval = int(3 + 5 * 10 / (10 - 5));

    info = get_expr_eval_source();

    EvalResult eval_out = eval_expr_string(expr_input, info);
    REQUIRE(eval_out.val == exp_eval);
    REQUIRE(eval_out.ok == true);
}

TEST_CASE("test_expr_eval_symbol_failure", "directive")
{
    SourceInfo info;
    const std::string expr_input = "3 + a * b / (y - 5)";

    info = get_expr_eval_source();

    EvalResult eval_out = eval_expr_string(expr_input, info);
    REQUIRE(eval_out.ok == false);
}
