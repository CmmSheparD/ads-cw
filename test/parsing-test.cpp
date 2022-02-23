#include "../src/parsing.hh"

#include <cmath>
#include <memory>
#include <string>

#include <gtest/gtest.h>

#include "../src/calculation-tree.hh"

using namespace std;
using namespace infix_parsing;
using namespace calculation;


/*
 * Parsing module heavily depends on the ParsingTable module init, so
 * this test always has to be run.
 */

TEST(Initial, Initialization)
{
    ASSERT_NO_THROW(init_table());
}

TEST(SingleValue, Value)
{
    shared_ptr<Operand> res;
    ASSERT_NO_THROW(res = parse_expression("2"));
    cout << res->evaluate() << endl;
    ASSERT_NO_THROW(res = parse_expression("0"));
    cout << res->evaluate() << endl;
    ASSERT_NO_THROW(res = parse_expression("2.3"));
    cout << res->evaluate() << endl;
    ASSERT_NO_THROW(res = parse_expression("pi"));
    cout << res->evaluate() << endl;
    ASSERT_NO_THROW(res = parse_expression("e"));
    cout << res->evaluate() << endl;
}

TEST(SingleValue, Brakes)
{
    shared_ptr<Operand> res;
    ASSERT_NO_THROW(res = parse_expression("(2)"));
    cout << res->evaluate() << endl;
    ASSERT_NO_THROW(res = parse_expression("((2))"));
    cout << res->evaluate() << endl;
    ASSERT_NO_THROW(res = parse_expression("(((2)))"));
    cout << res->evaluate() << endl;
    ASSERT_NO_THROW(res = parse_expression("((((2))))"));
    cout << res->evaluate() << endl;
}

TEST(SimpleExpression, Unary)
{
    shared_ptr<Operand> res;
    ASSERT_NO_THROW(res = parse_expression("-2"));
    ASSERT_EQ(res->evaluate(), -2);
    ASSERT_NO_THROW(res = parse_expression("--2"));
    ASSERT_EQ(res->evaluate(), 2);
    ASSERT_NO_THROW(res = parse_expression("-(2)"));
    ASSERT_EQ(res->evaluate(), -2);
    ASSERT_NO_THROW(res = parse_expression("-(-2)"));
    ASSERT_EQ(res->evaluate(), 2);
}

TEST(SimpleExpression, Binary)
{
    shared_ptr<Operand> res;
    ASSERT_NO_THROW(res = parse_expression("3-2"));
    ASSERT_EQ(res->evaluate(), 1);
    ASSERT_NO_THROW(res = parse_expression("3*2"));
    ASSERT_EQ(res->evaluate(), 6);
    ASSERT_NO_THROW(res = parse_expression("3+2"));
    ASSERT_EQ(res->evaluate(), 5);
    ASSERT_NO_THROW(res = parse_expression("3/1"));
    ASSERT_EQ(res->evaluate(), 3);
    ASSERT_NO_THROW(res = parse_expression("3^3"));
    ASSERT_EQ(res->evaluate(), 27);
}

TEST(ComplexExpression, NoBrakes)
{
    shared_ptr<Operand> res;
    ASSERT_NO_THROW(res = parse_expression("3-2*3"));
    ASSERT_EQ(res->evaluate(), -3);
    ASSERT_NO_THROW(res = parse_expression("3*2+3"));
    ASSERT_EQ(res->evaluate(), 9);
    ASSERT_NO_THROW(res = parse_expression("3*2/3"));
    ASSERT_EQ(res->evaluate(), 2);
    ASSERT_NO_THROW(res = parse_expression("3^3*3"));
    ASSERT_EQ(res->evaluate(), 81);
}

TEST(ComplexExpression, Brakes)
{
    shared_ptr<Operand> res;
    ASSERT_NO_THROW(res = parse_expression("(3-2)*3"));
    ASSERT_EQ(res->evaluate(), 3);
    ASSERT_NO_THROW(res = parse_expression("3^(2*3)"));
    ASSERT_EQ(res->evaluate(), 729);
}

TEST(ComplexExpression, Functions)
{
    shared_ptr<Operand> res;
    ASSERT_NO_THROW(res = parse_expression("cos pi"));
    ASSERT_EQ(res->evaluate(), -1);
    ASSERT_NO_THROW(res = parse_expression("log 10 - 1"));
    ASSERT_EQ(res->evaluate(), 0);
    ASSERT_NO_THROW(res = parse_expression("log (10 * 10) "));
    ASSERT_EQ(res->evaluate(), 2);
    ASSERT_NO_THROW(res = parse_expression("sin (pi/6)"));
    ASSERT_TRUE(abs(res->evaluate()) - abs(0.5) < numeric_limits<double>::epsilon());
}
