/*
 * EXPRESSION
 * Stuff for parsing expressions. For instance, with .defb or .defw we want to be 
 * able to write (from the Mandlebrot example)
 *
 * x_end .defw  5 * scale / 10
 *
 * And be able to evaluate (5 * scale / 10) at assembly time
 */

#ifndef __EXPRESSION_HPP
#define __EXPRESSION_HPP

#include <string>
#include <utility>
#include <unordered_map>
#include "Source.hpp"           // for Token object


typedef enum 
{
    TOK_NULL,
    TOK_LITERAL,
    TOK_STRING,
    TOK_LEFT_PAREN,
    TOK_RIGHT_PAREN,
    TOK_PLUS,
    TOK_MINUS,
    TOK_STAR,
    TOK_SLASH
} ExprTokenType;


/*
 * tok_char_to_type()
 */
inline ExprTokenType tok_char_to_type(const char tok_char)
{
    if(std::isalpha(tok_char))
        return TOK_STRING;
    if(std::isdigit(tok_char))
        return TOK_LITERAL;

    switch(tok_char)
    {
        case '+':
            return TOK_PLUS;
        case '-':
            return TOK_MINUS;
        case '*':
            return TOK_STAR;
        case '/':
            return TOK_SLASH;
        case '(':
            return TOK_LEFT_PAREN;
        case ')':
            return TOK_RIGHT_PAREN;
        default:
            return TOK_NULL;
    }

    return TOK_NULL;        // shut compiler/linter up
}

/*
 * ExprToken
 * Token for an expression. Note that this conceptually duplicates the functionality 
 * of a Token, so consider consolidating those two things later...
 */
struct ExprToken
{
    ExprTokenType type;
    int           val;
    std::string   repr;

    public:
        ExprToken();
        ExprToken(const ExprTokenType t, int val, const std::string& r);
        ExprToken(const ExprToken& that) = default;

        bool operator==(const ExprToken& that) const;
        bool operator!=(const ExprToken& that) const;
        bool isOperator(void) const;
        bool isParen(void) const;
        std::string toString(void) const;
};

/*
 * Expression.
 * Holds an expression string and some result which the expresion evaluates
 * to. In this case we only care about expressions which can be reduces to 
 * some plain old datatype (usually ints) rather than more general expressions
 */
// TODO: inherit from Token? (eg: ExprToken)?
struct Expression
{
    std::string expr_string;
    float eval;
    // TODO : maybe a vector of ExprTokens?

    public:
        Expression();
        Expression(const std::string& expr, float val);

        bool operator==(const Expression& that) const;
        bool operator!=(const Expression& that) const;
        int  evalInt(void) const;

        std::string toString(void) const;
};


/*
 * Precedence map 
 */
enum class Assoc { none, left_to_right, right_to_left };

struct OpInfo
{
    int prec;
    Assoc assoc;

    public:
        OpInfo() : prec(0), assoc(Assoc::none) {} 
        OpInfo(int p, Assoc a) : prec(p), assoc(a) {} 
        OpInfo(const OpInfo& that) = default;

        bool operator==(const OpInfo& that) const;
        bool operator!=(const OpInfo& that) const;
        std::string toString(void) const;
};


// TODO: xor? 
static std::unordered_map <ExprTokenType, OpInfo> OP_INFO_MAP = {
    {TOK_PLUS,  OpInfo(2, Assoc::left_to_right)},
    {TOK_MINUS, OpInfo(2, Assoc::left_to_right)},
    {TOK_STAR,  OpInfo(3, Assoc::left_to_right)},
    {TOK_SLASH, OpInfo(4, Assoc::left_to_right)},
};


/*
 * ParseResult
 * Output from a single call to expr_next_token. Holds the 
 * extracted ExprTokenType and the position in the stream 
 * from which to read the next token
 */
struct ParseResult
{
    ExprToken token;
    int       pos;

    public:
        ParseResult();
        ParseResult(const ExprToken& tok, int p);
        ParseResult(const ParseResult& that) = default;
        bool operator==(const ParseResult& that) const;
        bool operator!=(const ParseResult& that) const;
        std::string toString(void) const;
};


/*
 * Scan string from offset and return a token
 */
ParseResult expr_next_token(const std::string& src, unsigned int offset);

/*
 * Parse an expression string and return an Expression object
 */
Expression eval_expr_string(const std::string& expr_string);

#endif /*__EXPRESSION_HPP*/
