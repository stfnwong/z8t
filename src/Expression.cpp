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
#include <stack>
#include "Expression.hpp"



// TODO
Expression::Expression() : expr_string(""), eval(0) {} 



std::string Expression::toString(void) const
{
    return this->expr_string + " " + std::to_string(this->eval);
}



/*
 * Scan string from offset and return a token
 */
std::pair<Token, int> next_token(const std::string& src, unsigned int offset)
{
    Token tok;
    unsigned int idx = offset;

    std::cout << "[" << __func__ << "] input from " << std::dec << offset << " is ["
        << src.substr(offset, src.length()) << "]" << std::endl;

    while(idx < src.length())
    {
        // check for seperators
        if(src[idx] == ',')
            break;
        if(src[idx] == ';')
            break;
        // eat whitespace
        if(src[idx] == ' ' || src[idx] == '\n')
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
    // in this instance we don't try to do any conversions. We can 
    // work "types" out later if need be

    return std::pair<Token, int>(tok, idx);
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
