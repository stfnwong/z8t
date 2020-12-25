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

ExprToken& ExprToken::operator=(const ExprToken& that)
{
    this->type = that.type;
    this->val  = that.val;
    this->repr = that.repr;

    return *this;
}

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
        case TOK_LPAREN:
        case TOK_RPAREN:
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
        case TOK_LPAREN:
        case TOK_RPAREN:
            return true;
        default:
            return false;
    }

    return false;   // shut linter up
}

/*
 * precedence()
 */
//int ExprToken::precedence(void) const
//{
//    return OP_INFO_MAP[this->type].prec;
//}


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
        case TOK_LPAREN:
            oss << "TOK_LPAREN";
            break;
        case TOK_RPAREN:
            oss << "TOK_RPAREN";
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

ParseResult& ParseResult::operator=(const ParseResult& that) 
{
    this->token = that.token;
    this->pos = that.pos;

    return *this;
}

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


/*
 * ExprStack
 */

// constructors
ExprStack::ExprStack() {} 
ExprStack::ExprStack(const std::vector<ExprToken>& s) : stack(s) {} 

// operators 
const ExprToken& ExprStack::operator[](unsigned int idx) const
{
    return this->stack[idx];
}

ExprToken& ExprStack::operator[](unsigned int idx)
{
    return this->stack[idx];
}

// note : may be slow...
bool ExprStack::operator==(const ExprStack& that) const
{
    if(this->size() != that.size())
        return false;
    for(unsigned int idx = 0; idx < this->size(); ++idx)
    {
        if(this->stack[idx] != that.stack[idx])
            return false;
    }

    return true;
}

bool ExprStack::operator!=(const ExprStack& that) const
{
    return !(*this == that);
}

// methods
void ExprStack::push(const ExprToken& t)
{
    this->stack.push_back(t);
}

const ExprToken& ExprStack::top(void)
{
    return this->stack.back();
}

ExprToken ExprStack::pop(void)
{
    ExprToken t = std::move(this->top());
    this->stack.pop_back();
    return t;
}

bool ExprStack::empty(void) const
{
    return this->stack.empty();
}

unsigned int ExprStack::size(void) const
{
    return this->stack.size();
}

std::string ExprStack::toString(void) const
{
    std::ostringstream oss;

    for(unsigned int idx = 0; idx < this->size(); ++idx)
    {
        oss << this->stack[idx].repr << " ";
    }

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
 * expr_tokenize()
 */
ExprStack expr_tokenize(const std::string& expr_string)
{
    ExprStack tok_stack;
    ParseResult parse_result;     // TODO: this could perhaps be reduced to a pair?

    while(parse_result.pos < expr_string.length())
    {
        // ==== Tokenize 
        parse_result = expr_next_token(expr_string, parse_result.pos);
        tok_stack.push(parse_result.token);
    }

    return tok_stack;
}

/*
 * expr_infix_to_postfix()
 */
ExprStack expr_infix_to_postfix(const ExprStack& infix_stack)
{
    ExprStack output_stack;
    ExprStack operator_stack;

    ExprToken cur_token;        // TODO : change scope to be inside loop?
    ExprToken op_tok;
    for(unsigned int tok_idx = 0; tok_idx < infix_stack.size(); ++tok_idx)
    {
        cur_token = infix_stack[tok_idx];
        if(cur_token.type == TOK_LITERAL)
        {
            output_stack.push(cur_token);
        }
        else if(cur_token.isOperator())
        {
            // TODO ; there are more conditions, but worry about those later
            //while(operator_stack.top().isOperator() && 
            //      operator_stack.top().type != TOK_LPAREN)
            //{
            //    op_tok = operator_stack.top();
            //    output_stack.push(op_tok);
            //    operator_stack.pop();
            //}
            
            while(!operator_stack.empty())
            {
                op_tok = operator_stack.top();
                // Enforce precendece rules
                //if(op_tok.isOperator() && op_tok.type != TOK_LPAREN)
                if(op_tok.type == TOK_RPAREN || 
                   (Precedence(op_tok.type) > Precedence(cur_token.type)) ||
                   ((Precedence(op_tok.type) == Precedence(cur_token.type)) && 
                    (Associativity(cur_token.type) == Assoc::left_to_right)))
                {
                    output_stack.push(op_tok);
                    operator_stack.pop();
                }
                else
                    break;
            }
            operator_stack.push(cur_token);
        }
        else if(cur_token.type == TOK_LPAREN)
            operator_stack.push(cur_token);
        else if(cur_token.type == TOK_RPAREN)
        {
            //while(operator_stack.top().type != TOK_LPAREN)
            //{
            //    op_tok = operator_stack.top();
            //    output_stack.push(op_tok);
            //    operator_stack.pop();
            //}
            while(!operator_stack.empty())
            {
                op_tok = operator_stack.top();
                if(op_tok.type != TOK_LPAREN)
                {
                    output_stack.push(op_tok);
                    operator_stack.pop();
                }
                else
                    break;
            }

            if(operator_stack.top().type == TOK_LPAREN)
                operator_stack.pop();       // discard
            // also would discard functions here, but for now functions aren't supported.
        }
        // TODO: debug, remove 
        std::cout << "[" << std::dec << std::setw(4) << tok_idx << "]" 
            << std::setw(6) << operator_stack.toString() << "   " << output_stack.toString() << std::endl;
    }

    // pop any remaining tokens on the operator stack to the output stack
    while(!operator_stack.empty())
    {
        op_tok = operator_stack.top();
        output_stack.push(op_tok);
        operator_stack.pop();
    }

    return output_stack;
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
            while(!stack.empty() && stack.top().isOperator() && (stack.top().type != TOK_LPAREN))
            {
                std::cout << "[" << __func__ << "] moving token " << stack.top().toString() << " to output stack " << std::endl;
                output.push(stack.top());
                stack.pop();
            }
            //if(cur_token.type != TOK_LPAREN)
            //{
            //    while(!stack.empty() && ((cur_token.type == TOK_RPAREN) && (stack.top().type != TOK_LPAREN)))
            //    {
            //        std::cout << "[" << __func__ << "] moving token " << stack.top().toString() << " to output stack " << std::endl;
            //        output.push(stack.top());
            //        stack.pop();
            //    }

            //    //while(!stack.empty() 
            //    //   && ((cur_token.type == TOK_RPAREN) && (stack.top().type != TOK_RIGHT_PAREN))
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
            //    if(cur_token.type == TOK_RPAREN)
            //        stack.pop();
            //}

            // Everything except the closing paren can be discarded
            if(cur_token.type != TOK_RPAREN)
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
