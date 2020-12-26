/*
 * EXPRESSION
 * Stuff for parsing expressions. For instance, with .defb or .defw we want to be 
 * able to write (from the Mandlebrot example)
 *
 * x_end .defw  5 * scale / 10
 *
 * And be able to evaluate (5 * scale / 10) at assembly time
 *
 * Everything is split apart which makes things slower. For instance, it would be faster if
 * we had access to the symbol table directly so that we could update string tokens as we
 * tokenize. I've decided to do this seperately for now mainly for testing purposes, but 
 * this sort of thing might be better off being in the Assembler component. Otherwise we need
 * to tokenize, then take the ExprStack, check if there are any strings, and for each of those 
 * strings call out to the symbol table and lookup, and potentially throw something/print an 
 * error message if the symbol isn't found.
 */

#ifndef __EXPRESSION_HPP
#define __EXPRESSION_HPP

#include <string>
#include <utility>
#include <unordered_map>
#include "Source.hpp"           // for Token object

//#define __EXPRESSION_DEBUG_PRINT


/*
 * Token types 
 */
typedef enum 
{
    TOK_NULL,
    TOK_LITERAL,
    TOK_STRING,     
    TOK_LPAREN,     // '('
    TOK_RPAREN,     // ')'
    TOK_PLUS,       // '+'
    TOK_MINUS,      // '-'
    TOK_STAR,       // '*'
    TOK_SLASH,      // '/'
    TOK_CARET       // '^'
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
            return TOK_LPAREN;
        case ')':
            return TOK_RPAREN;
        default:
            return TOK_NULL;
    }

    return TOK_NULL;        // shut compiler/linter up
}

// TODO: xor? 
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

// Mapping of precedence for each operator
static std::unordered_map <ExprTokenType, OpInfo> OP_INFO_MAP = 
{
    {TOK_PLUS,  OpInfo(2, Assoc::left_to_right)},
    {TOK_MINUS, OpInfo(2, Assoc::left_to_right)},
    {TOK_STAR,  OpInfo(3, Assoc::left_to_right)},
    {TOK_SLASH, OpInfo(3, Assoc::left_to_right)},
    {TOK_CARET, OpInfo(4, Assoc::right_to_left)}
};

inline int Precedence(const ExprTokenType& tok_type)
{
    return OP_INFO_MAP[tok_type].prec;
}

inline Assoc Associativity(const ExprTokenType& tok_type)
{
    return OP_INFO_MAP[tok_type].assoc;
}

/*
 * ExprToken
 * Token for an expression. Note that this conceptually duplicates the functionality 
 * of a Token, so consider consolidating those two things later...
 */
struct ExprToken
{
    ExprTokenType type;
    std::string   repr;

    public:
        ExprToken();
        ExprToken(const ExprTokenType t, const std::string& r);
        ExprToken(const ExprToken& that); 
        ExprToken(ExprToken&& that);

        ExprToken& operator=(const ExprToken& that);        // TODO: default?
        ExprToken& operator=(ExprToken&& that);
        bool operator==(const ExprToken& that) const;
        bool operator!=(const ExprToken& that) const;
        bool isOperator(void) const;
        bool isParen(void) const;
        float literal(void) const;

        std::string toString(void) const;
};


/*
 * ParseResult
 * Output from a single call to expr_next_token. Holds the 
 * extracted ExprTokenType and the position in the stream 
 * from which to read the next token
 */
struct ParseResult
{
    ExprToken    token;
    unsigned int pos;

    public:
        ParseResult();
        ParseResult(const ExprToken& tok, int p);
        ParseResult(const ParseResult& that) = default;
        ParseResult& operator=(const ParseResult& that);
        bool operator==(const ParseResult& that) const;
        bool operator!=(const ParseResult& that) const;
        std::string toString(void) const;
};

/*
 * Expression stack based on a vector.
 * This should give better iteration, printing, etc
 */
struct ExprStack
{
    std::vector<ExprToken> stack;

    public:
        ExprStack();
        ExprStack(const std::vector<ExprToken>& s);
        ExprStack(const ExprStack& that) = default;

        const ExprToken& operator[](unsigned int idx) const;
        ExprToken& operator[](unsigned int idx);
        bool operator==(const ExprStack& that) const;
        bool operator!=(const ExprStack& that) const;

        void             push(const ExprToken& t);
        const ExprToken& top(void);
        ExprToken        pop(void);
        bool             empty(void) const;
        unsigned int     size(void) const;

        std::string      toString(void) const;
};

/*
 * Scan string from offset and return a token
 */
ParseResult expr_next_token(const std::string& src, unsigned int offset);
/*
 * Tokenize a string into an ExprStack
 */
ExprStack expr_tokenize(const std::string& expr_string);

/*
 * expr_infix_to_postfix()
 * Take a stack containing expression tokens in infix notation
 * and transform it into a stack of tokens in postfix notation
 */
ExprStack expr_infix_to_postfix(const ExprStack& infix_stack);

/*
 * eval_postfix_expr_stack
 * Take an ExprStack containing a postfix expression and evaluate it
 */
float eval_postfix_expr_stack(const ExprStack& expr_stack);

/*
 * eval_expr_string()
 */
float eval_expr_string(const std::string& expr_string);

/*
 * display_stack_debug()
 */
inline void display_stack_debug(
        unsigned int idx, 
        const ExprToken& cur_token, 
        const ExprStack& out_stack,
        const ExprStack& op_stack,
        const std::string& end_str
        )
{
    std::cout << "[" << std::dec << std::setw(4) << idx << "]" 
        << std::setw(6) << op_stack.toString() << "    " 
        << std::setw(20) << out_stack.toString() << "   "
        << cur_token.toString() 
        << end_str << std::endl;
}


#endif /*__EXPRESSION_HPP*/
