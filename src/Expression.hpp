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
        std::string toString(void) const;
};

/*
 * Expression.
 * Holds an expression string and some result which the expresion evaluates
 * to. In this case we only care about expressions which can be reduces to 
 * some plain old datatype (usually ints) rather than more general expressions
 */
// TODO: inherit from Token? (eg: ExprToken)?
// TODO : should this be templated (to make typed results simpler...)?
template <typename T> struct Expression
{
    std::string expr_string;
    T eval;
    // TODO : maybe a vector of ExprTokens?

    public:
        Expression();
        Expression(const std::string& expr, T val);

        bool operator==(const Expression& that) const;
        bool operator!=(const Expression& that) const;

        std::string toString(void) const;
};



// ======== EXPRESSION ======== //
template <typename T> Expression<T>::Expression() : expr_string(""), eval(0) {} 

template <typename T> Expression<T>::Expression(const std::string& expr, T val) : expr_string(expr), eval(val) {} 

/*
 * Expression::==
 */
template <typename T> bool Expression<T>::operator==(const Expression& that) const
{
    if(this->expr_string != that.expr_string)
        return false;
    if(this->eval != that.eval)
        return false;

    return true;
}

/*
 * Expression::!=
 */
template <typename T> bool Expression<T>::operator!=(const Expression& that) const
{
    return !(*this == that);
}

/*
 * toString()
 */
template <typename T> std::string Expression<T>::toString(void) const
{
    return this->expr_string + " " + std::to_string(this->eval);
}



/*
 * Scan string from offset and return a token
 */
std::pair<ExprToken, int> next_token(const std::string& src, unsigned int offset);

/*
 * Parse an expression string and return an Expression object
 */
template <typename T> Expression<T> eval_expr_string(const std::string& expr_string, T force_template)
{
    Expression<T> expr;
    ExprToken top_token;
    // token stacks 
    std::stack<ExprToken> output_stack;
    std::stack<ExprToken> op_stack;

    int idx = 0;

    std::pair<ExprToken, int> out_pair;

    while(idx < expr_string.length())
    {
        out_pair = next_token(expr_string, out_pair.second);
        idx += out_pair.second;

        ExprToken cur_token = out_pair.first;

        if(cur_token.type == TOK_LITERAL)
            output_stack.push(cur_token);
        else if(cur_token.isOperator())
        {
            while(!op_stack.empty())
            {
                // TODO : need to implement left-associativity
                top_token = op_stack.top();
                if(top_token.type != TOK_LEFT_PAREN)
                {
                    output_stack.push(top_token);
                    op_stack.pop();
                }
            }
            op_stack.push(cur_token);
        }
        else if(cur_token.type == TOK_LEFT_PAREN)
            op_stack.push(cur_token);
        else if(cur_token.type == TOK_RIGHT_PAREN)
        {
            do
            {
                top_token = op_stack.top();
                if(top_token.type != TOK_LEFT_PAREN)
                {
                    output_stack.push(top_token);
                    op_stack.pop();
                }
            } while(top_token.type != TOK_LEFT_PAREN);
            
            // if there is still a left paren then there is a missing right paren. 
            // discard the top operator (and ideally emit some error message)
            top_token = op_stack.top();
            if(top_token.type == TOK_LEFT_PAREN)
                op_stack.pop();     // TODO: also some error, traceback, etc...
        }

        // TODO: eval here?
    }

    while(!op_stack.empty())
    {
        top_token = op_stack.top();
        output_stack.push(top_token);
        op_stack.pop();
    }

    // For now, just print the stack 
    idx = 0;
    while(!output_stack.empty())
    {
        top_token = output_stack.top();
        std::cout << "[" << __func__ << "] sp " << std::dec << idx << " : " << top_token.toString() << std::endl;
        output_stack.pop();
        idx++;
    }

    return expr;
}

#endif /*__EXPRESSION_HPP*/
