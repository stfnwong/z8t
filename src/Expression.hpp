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
    int eval;

    public:
        Expression();

        std::string toString(void) const;

};

/*
 * Scan string from offset and return a token
 */
std::pair<Token, int> next_token(const std::string& src, unsigned int offset);

/*
 * Parse an expression string and return an Expression object
 */
Expression eval_expr_string(const std::string& expr_string, int offset);



#endif /*__EXPRESSION_HPP*/
