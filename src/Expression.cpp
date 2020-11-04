/*
 * EXPRESSION
 * Stuff for parsing expressions. For instance, with .defb or .defw we want to be 
 * able to write (from the Mandlebrot example)
 *
 * x_end .defw  5 * scale / 10
 *
 * And be able to evaluate (5 * scale / 10) at assembly time
 */

#include <iostream>
#include <iomanip>
#include <stack>
#include <sstream>
#include "Expression.hpp"


// ======== EXPRTOKEN ======== //
ExprToken::ExprToken() : type(TOK_NULL), val(0), repr("") {} 

ExprToken::ExprToken(const ExprTokenType t, int v, const std::string& r) : type(t), val(v), repr(r) {}

/*
 * ==
 */
bool ExprToken::operator==(const ExprToken& that) const
{
    if(this->type != that.type)
        return false;
    if(this->val != that.val)
        return false;
    if(this->repr != that.repr)
        return false;

    return true;
}

/*
 * !=
 */
bool ExprToken::operator!=(const ExprToken& that) const
{
    return !(*this == that);
}

/*
 * toString()
 */
std::string ExprToken::toString(void) const
{
    std::ostringstream oss;

    switch(this->type)
    {
        case TOK_LITERAL:
            oss << "TOK_LITERAL";
            break;
        case TOK_STRING:
            oss << "TOK_STRING";
            break;
        case TOK_LEFT_PAREN:
            oss << "TOK_LEFT_PAREN";
            break;
        case TOK_RIGHT_PAREN:
            oss << "TOK_RIGHT_PAREN";
            break;
        case TOK_PLUS:
            oss << "TOK_PLUS";
            break;
        case TOK_MINUS:
            oss << "TOK_MINUS";
            break;
        case TOK_STAR:
            oss << "TOK_STAR";
            break;
        case TOK_SLASH:
            oss << "TOK_SLASH";
            break;
        default:
            oss << "TOK_NULL";
            break;
    }
    oss << "<" << this->repr << ">  " << std::dec << this->val;

    return oss.str();
}


// ======== EXPRESSION ======== //
Expression::Expression() : expr_string(""), eval(0) {} 

bool Expression::operator==(const Expression& that) const
{
    if(this->expr_string != that.expr_string)
        return false;
    if(this->eval != that.eval)
        return false;

    return true;
}

bool Expression::operator!=(const Expression& that) const
{
    return !(*this == that);
}

/*
 * toString()
 */
std::string Expression::toString(void) const
{
    return this->expr_string + " " + std::to_string(this->eval);
}


/*
 * Scan string from offset and return a token
 */
std::pair<ExprToken, int> next_token(const std::string& src, unsigned int offset)
{
    ExprToken tok;
    unsigned int idx = offset;

    std::cout << "[" << __func__ << "] input from offset (" << std::dec << idx << ") is \""
        << src.substr(offset, src.length()) << "\"" << std::endl;

    while(idx < src.length())
    {
        // check for seperators
        if(src[idx] == ',')
            break;
        if(src[idx] == ';')
            break;
        // eat whitespace
        if(src[idx] == ' ' || src[idx] == '\n' || src[idx] == '\t')
        {
            idx++;
            continue;
        }
        // these are actually tokens themselves, so rather than split 
        // on them we want to extract into a new Token object 
        if(src[idx] == '(' || src[idx] == ')')
        {
            if(tok.repr.length() == 0)
            {
                tok.repr.push_back(src[idx]);
                idx++;
            }
            break;
        }
        // arithmetic 
        if(src[idx] == '/' || src[idx] == '*' || src[idx] == '+' || src[idx] == '-')
        {
            if(tok.repr.length() == 0)
            {
                tok.repr.push_back(src[idx]);
                idx++;
            }
            break;
        }

        tok.repr.push_back(src[idx]);
        idx++;
    }
    tok.type = tok_char_to_type(tok.repr[0]);

    return std::pair<ExprToken, int>(tok, idx);
}


/*
 * Parse an expression string and return an Expression object
 */
Expression eval_expr_string(const std::string& expr_string)
{
    Expression expr;
    std::stack<std::string> paren_stack;


    return expr;
}
