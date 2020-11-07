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
 * isOperator()
 */
bool ExprToken::isOperator(void) const
{
    switch(this->type)
    {
        case TOK_LEFT_PAREN:
        case TOK_RIGHT_PAREN:
        case TOK_PLUS:
        case TOK_MINUS:
        case TOK_STAR:
        case TOK_SLASH:
            return true;
        default:
            return false;
    }

    return false;       // shut linter up
}

/*
 * isParen()
 */
bool ExprToken::isParen(void) const
{
    switch(this->type)
    {
        case TOK_LEFT_PAREN:
        case TOK_RIGHT_PAREN:
            return true;
        default:
            return false;
    }

    return false;   // shut linter up
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

Expression::Expression(const std::string& expr, float val) : expr_string(expr), eval(val) {} 

/*
 * Expression::==
 */
bool Expression::operator==(const Expression& that) const
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
bool Expression::operator!=(const Expression& that) const
{
    return !(*this == that);
}

int Expression::evalInt(void) const
{
    return static_cast<int>(this->eval);     
}

/*
 * toString()
 */
std::string Expression::toString(void) const
{
    return this->expr_string + " " + std::to_string(this->eval);
}


/*
 * next_expr_token()
 * Scan string from offset and return a token
 */
std::pair<ExprToken, int> next_expr_token(const std::string& src, unsigned int offset)
{
    ExprToken tok;
    unsigned int idx = offset;

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
 * eval_expr_string()
 * Parse an expression string and return an Expression object
 */
Expression eval_expr_string(const std::string& expr_string)
{
    Expression expr;
    ExprToken top_token;
    // token stacks (TODO : does it turn out to be faster/better to use vectors here?)
    std::stack<ExprToken> output_stack;
    std::stack<ExprToken> op_stack;

    unsigned int idx = 0;
    std::pair<ExprToken, int> out_pair;

    while(idx < expr_string.length())
    {
        out_pair = next_expr_token(expr_string, out_pair.second);
        idx += out_pair.second;

        ExprToken cur_token = out_pair.first;

        if(cur_token.type == TOK_LITERAL)
            output_stack.push(cur_token);
        else if(cur_token.isOperator() || cur_token.isParen())
        {
            // TODO : debug, remove 
            std::cout << "[" << __func__ << "] adding token " << cur_token.toString() << " to stack..." << std::endl;
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
