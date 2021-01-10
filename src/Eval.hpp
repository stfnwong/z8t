/*
 * EVAL
 * Stuff common for expression evaluation
 */

#ifndef __EVAL_HPP
#define __EVAL_HPP

#include <string>

/*
 * EvalResult()
 */
struct EvalResult
{
    int  val;
    bool ok;

    public:
        EvalResult();
        EvalResult(const int v);
        EvalResult(const int v, const bool o);
        EvalResult(const EvalResult& that) = default;

        void init(void);
        EvalResult& operator=(const EvalResult& that);
        bool operator==(const EvalResult& that) const;
        bool operator!=(const EvalResult& that) const;
        EvalResult& operator=(const EvalResult& that) const;
        std::string toString(void) const;
};


#endif /*__EVAL_HPP*/
