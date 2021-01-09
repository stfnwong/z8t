/*
 * TEST_EVAL
 * Tests for objects in Eval.cpp
 */


#define CATCH_CONFIG_MAIN
#include "catch/catch.hpp"

#include <iostream> 

#include "Eval.hpp"

TEST_CASE("test_eval_result_init", "eval")
{
    EvalResult er;

    REQUIRE(er.val == 0);
    REQUIRE(er.ok == false);

    EvalResult res2(10, true);

    REQUIRE(res2.val == 10);
    REQUIRE(res2.ok == true);
    
    std::cout << res2.toString() << std::endl;

    res2.init();
    REQUIRE(res2.val == 0);
    REQUIRE(res2.ok == false);
}
