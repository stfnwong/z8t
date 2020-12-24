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
    oss << " <" << this->repr << ">  " << std::dec << this->val;

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
 * OP_INFO
 */
bool OpInfo::operator==(const OpInfo& that) const
{
    if(this->prec != that.prec)
        return false;
    if(this->assoc != that.assoc)
        return false;
    return true;
}

bool OpInfo::operator!=(const OpInfo& that) const
{
    return !(*this == that);
}

std::string OpInfo::toString(void) const
{
    switch(this->assoc)
    {
        case Assoc::left_to_right:
            return "Left-to-Right (" + std::to_string(this->prec) + ")";
        case Assoc::right_to_left:
            return "Right-to-Left (" + std::to_string(this->prec) + ")";
        default:
            return "None (" + std::to_string(this->prec) + ")";
    }

    return "None (" + std::to_string(this->prec) + ")";
}


/*
 * PARSE_RESULT
 */
ParseResult::ParseResult() : token(ExprToken()), pos(0) {} 

ParseResult::ParseResult(const ExprToken& tok, int p) : token(tok), pos(p) {} 

bool ParseResult::operator==(const ParseResult& that) const
{
    if(this->token != that.token)
        return false;
    if(this->pos != that.pos)
        return false;
    
    return true;
}

bool ParseResult::operator!=(const ParseResult& that) const
{
    return !(*this == that);
}

std::string ParseResult::toString(void) const
{
    std::ostringstream oss;

    oss << this->token.toString() << " next pos: [" << std::dec << this->pos << "]";

    return oss.str();
}


// ======== PARSING FUNCTIONS ======== //

/*
 * expr_next_token()
 * Scan string from offset and return a token
 */
ParseResult expr_next_token(const std::string& src, unsigned int offset)
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

    return ParseResult(tok, idx);
}


/*
 * eval_expr_string()
 * Parse an expression string and return an Expression object
 */
Expression eval_expr_string(const std::string& expr_string)
{
    Expression expr;
    ExprToken top_token;
    ExprToken cur_token;
    // token stacks (TODO : does it turn out to be faster/better to use vectors here?)
    std::stack<ExprToken> output;
    std::stack<ExprToken> stack;

    unsigned int idx = 0;
    ParseResult parse_result;

    std::cout << "[" << __func__ << "] expr string contains " << expr_string.length() << " characters" << std::endl;

    // TODO: need to implement shunting algorithm here...
    while(parse_result.pos < expr_string.length())
    {
        // ==== Tokenize 
        parse_result = expr_next_token(expr_string, parse_result.pos);
        //idx += parse_result.pos;

        std::cout << "[" << __func__ << "] parse result is: " << parse_result.toString() 
            << ", idx is " << idx << std::endl;

        cur_token = parse_result.token;

        if(cur_token.type == TOK_LITERAL)
        {
            std::cout << "[" << __func__ << "] pushing token " << cur_token.toString() << " to output stack" << std::endl;
            output.push(cur_token);
        }
        else if(cur_token.isOperator() || cur_token.isParen())
        {
            while(!stack.empty() && stack.top().isOperator() && (stack.top().type != TOK_LEFT_PAREN))
            {
                std::cout << "[" << __func__ << "] moving token " << stack.top().toString() << " to output stack " << std::endl;
                output.push(stack.top());
                stack.pop();
            }
            //if(cur_token.type != TOK_LEFT_PAREN)
            //{
            //    while(!stack.empty() && ((cur_token.type == TOK_RIGHT_PAREN) && (stack.top().type != TOK_LEFT_PAREN)))
            //    {
            //        std::cout << "[" << __func__ << "] moving token " << stack.top().toString() << " to output stack " << std::endl;
            //        output.push(stack.top());
            //        stack.pop();
            //    }

            //    //while(!stack.empty() 
            //    //   && ((cur_token.type == TOK_RIGHT_PAREN) && (stack.top().type != TOK_RIGHT_PAREN))
            //    //   || (Precedence(stack.top().type) > Precedence(cur_token.type))
            //    //   || (Precedence(stack.top().type) == Precedence(cur_token.type))
            //    //   && (Associativity(cur_token.type) == Assoc::left_to_right)
            //    //   )
            //    //{
            //    //    top_token = stack.top();
            //    //    output.push(top_token);
            //    //    stack.pop();
            //    //}

            //    // if we popped until '(' (because the token is a ')') then discard parens
            //    if(cur_token.type == TOK_RIGHT_PAREN)
            //        stack.pop();
            //}

            // Everything except the closing paren can be discarded
            if(cur_token.type != TOK_RIGHT_PAREN)
            {
                std::cout << "[" << __func__ << "] pushing token " << cur_token.toString() 
                    << " to stack... " << std::endl;
                stack.push(cur_token);
            }
        }
        else
        {
            std::cout << "[" << __func__ << "] unexpected token " << cur_token.toString() << " at index " 
                << std::dec << parse_result.pos << std::endl;
        }
    }

    // Any remaining tokens are just moved directly to the output
    while(!stack.empty())
    {
        top_token = stack.top();
        output.push(top_token);
        stack.pop();
    }

    // For now, just print the stack 
    idx = 0;
    while(!output.empty())
    {
        top_token = output.top();
        std::cout << "[" << __func__ << "] sp " << std::dec << idx << " : " << top_token.toString() << std::endl;
        output.pop();
        idx++;
    }

    return expr;
}
