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


// ======== EXPRTOKEN ======== //
ExprToken::ExprToken() : type(TOK_NULL), repr("") {} 

ExprToken::ExprToken(const ExprTokenType t, const std::string& r) : type(t), repr(r) {}

ExprToken::ExprToken(const ExprToken& that) : type(that.type), repr(that.repr) {} 

ExprToken::ExprToken(ExprToken&& that) : type(TOK_NULL), repr("")
{
    std::swap(this->type, that.type);
    std::swap(this->repr, that.repr);
}

ExprToken& ExprToken::operator=(const ExprToken& that)
{
    // TODO: can we reduce this to
    // std::swap(*this, that);  ? 
    this->type = that.type;
    this->repr = that.repr;

    return *this;
}

ExprToken& ExprToken::operator=(ExprToken&& that)
{
    if(this != &that)
    {
        this->type = std::move(that.type);
        this->repr = std::move(that.repr);
    }

    return *this;
}

/*
 * ExprToken::==
 */
bool ExprToken::operator==(const ExprToken& that) const
{
    if(this->type != that.type)
        return false;
    if(this->repr != that.repr)
        return false;

    return true;
}

/*
 * ExprToken::!=
 */
bool ExprToken::operator!=(const ExprToken& that) const
{
    return !(*this == that);
}

/*
 * ExprToken::isOperator()
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
 * ExprToken::isParen()
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
 * ExprToken::literal()
 * Return a literal value for this token
 */
float ExprToken::literal(void) const
{
    if(this->type == TOK_LITERAL)
        return std::stoi(this->repr);

    return 0.0;
}

/*
 * ExprToken::toString()
 */
std::string ExprToken::toString(void) const
{
    std::ostringstream oss;

    switch(this->type)
    {
        case TOK_LITERAL:
            oss << std::setw(12) << "TOK_LITERAL";
            break;
        case TOK_STRING:
            oss << std::setw(12) << "TOK_STRING";
            break;
        case TOK_LPAREN:
            oss << std::setw(12) << "TOK_LPAREN";
            break;
        case TOK_RPAREN:
            oss << std::setw(12) << "TOK_RPAREN";
            break;
        case TOK_PLUS:
            oss << std::setw(12) << "TOK_PLUS";
            break;
        case TOK_MINUS:
            oss << std::setw(12) << "TOK_MINUS";
            break;
        case TOK_STAR:
            oss << std::setw(12) << "TOK_STAR";
            break;
        case TOK_SLASH:
            oss << std::setw(12) << "TOK_SLASH";
            break;
        default:
            oss << std::setw(12) << "TOK_NULL";
            break;
    }
    oss << " <" << this->repr << ">  ";

    return oss.str();
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
 * An Expression stack
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
    //ExprToken t = this->top();  // TODO: fix move semantics
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
        // check for numerics (for now we only support base 10 numbers
        tok.repr.push_back(src[idx]);
        idx++;
    }
    tok.type = tok_char_to_type(tok.repr[0]);

    return ParseResult(tok, idx);
}


/*
 * expr_tokenize()
 * Tokenize a string into an ExprStack
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

    ExprToken cur_token;        
    ExprToken op_tok;
    for(unsigned int tok_idx = 0; tok_idx < infix_stack.size(); ++tok_idx)
    {
        cur_token = infix_stack[tok_idx];
        if(cur_token.type == TOK_LITERAL)
        {
            output_stack.push(cur_token);
        }
        else if(cur_token.isOperator() || cur_token.isParen())
        {
            if(cur_token.type != TOK_LPAREN)
            {
                // pop operators to the output stack 
                // - if the current token is a right paren, pop until we see a left paren
                // - if the precedences of the current token is greater than the precedence
                // of the token at the top of the operator stack
                // - if the precendence of the current token is equal to the precedence
                // of the token at the top of the operator stack
                while(!operator_stack.empty())
                {
                    op_tok = operator_stack.top();
                    // Enforce precendece rules
                    if((op_tok.type == TOK_RPAREN && op_tok.type != TOK_LPAREN) || 
                       (Precedence(op_tok.type) > Precedence(cur_token.type)) ||
                       ((Precedence(op_tok.type) == Precedence(cur_token.type)) && 
                        (Associativity(cur_token.type) == Assoc::left_to_right)))
                    {
                        output_stack.push(op_tok);
                        operator_stack.pop();
#ifdef __EXPRESSION_DEBUG_PRINT
                        display_stack_debug(tok_idx, cur_token, output_stack, operator_stack, " op -> output ");
#endif /*__EXPRESSION_DEBUG_PRINT*/
                    }
                    else
                        break;
                }

                //operator_stack.push(cur_token);
                // if this is part of a paren pair, discard this parent
                if(cur_token.type == TOK_RPAREN)
                {
                    operator_stack.pop();       
#ifdef __EXPRESSION_DEBUG_PRINT
                    display_stack_debug(tok_idx, cur_token, output_stack, operator_stack, " pop operator ");
#endif /*__EXPRESSION_DEBUG_PRINT*/
                }
            }

            if(cur_token.type != TOK_RPAREN)
            {
                operator_stack.push(cur_token);
#ifdef __EXPRESSION_DEBUG_PRINT
                display_stack_debug(tok_idx, cur_token, output_stack, operator_stack, " push operator ");
#endif /*__EXPRESSION_DEBUG_PRINT*/

            }
        }
#ifdef __EXPRESSION_DEBUG_PRINT
        display_stack_debug(tok_idx, cur_token, output_stack, operator_stack, "");
#endif /*__EXPRESSION_DEBUG_PRINT*/
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
 * eval_postfix_expr_stack()
 */
float eval_postfix_expr_stack(const ExprStack& expr_stack)
{
    float l, r, y;
    std::stack<float> output_stack;

    for(unsigned int idx = 0; idx < expr_stack.size(); ++idx)
    {
        ExprToken cur_token = expr_stack[idx];

        if(cur_token.type == TOK_LITERAL)
            output_stack.push(cur_token.literal());
        if(cur_token.isOperator())
        {
            if(output_stack.size() < 2)
                return 0.0;     // TODO : how to signal invalid op? throw here?

            r = output_stack.top();
            output_stack.pop();
            l = output_stack.top();
            output_stack.pop();
            switch(cur_token.type)
            {
                case TOK_PLUS:
                    y = l + r;
                    break;
                case TOK_MINUS:
                    y = l - r;
                    break;
                case TOK_STAR:
                    y = l * r;
                    break;
                case TOK_SLASH:
                    y = l / r;
                    break;
            }
            output_stack.push(y);
        }
    }
    y = output_stack.top();

    return y;
}
