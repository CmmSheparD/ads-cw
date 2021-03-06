#include "../src/parsing-table.hh"

#include <cmath>
#include <functional>
#include <string>

#include <gtest/gtest.h>

#include "../src/calculation-tree.hh"

using namespace std;
using namespace calculation;
using pt = infix_parsing::ParsingTable;

/*
 * Since ParsingTable is a global state class. Every test will affect it's
 * state, so ordering is important.
 */

TEST(NameValidation, ZeroLen)
{
    ASSERT_FALSE(pt::is_valid_name(""));
}

TEST(NameValidation, InvNameStart)
{
    ASSERT_FALSE(pt::is_valid_name(" abc"));
    ASSERT_FALSE(pt::is_valid_name("0abc"));
}

TEST(NameValidation, NonPrintName)
{
    ASSERT_FALSE(pt::is_valid_name("a b"));
    ASSERT_FALSE(pt::is_valid_name("a\tb"));
}

TEST(NameValidation, AcceptedNames)
{
    ASSERT_TRUE(pt::is_valid_name("+"));
    ASSERT_TRUE(pt::is_valid_name("^"));
    ASSERT_TRUE(pt::is_valid_name("a"));
    ASSERT_TRUE(pt::is_valid_name("abc"));
    ASSERT_TRUE(pt::is_valid_name("a_c"));
    ASSERT_TRUE(pt::is_valid_name("a0"));
    ASSERT_TRUE(pt::is_valid_name("ac"));
}


TEST(ConstantSearch, NonExistent)
{
    ASSERT_NO_THROW(pt::is_constant("pi"));
    ASSERT_FALSE(pt::is_constant("pi"));
}

TEST(SearchUnary, NonExistent)
{
    ASSERT_NO_THROW(pt::is_unary_operator("sin"));
    ASSERT_FALSE(pt::is_unary_operator("sin"));
}

TEST(SearchBinary, NonExistent)
{
    ASSERT_NO_THROW(pt::is_binary_operator("+"));
    ASSERT_FALSE(pt::is_binary_operator("+"));
}

TEST(RegisterAndFind, Constants)
{
    ASSERT_NO_THROW(pt::register_constant("pi", 3.1415926535));
    ASSERT_TRUE(pt::is_constant("pi"));
    ASSERT_EQ(pt::get_constant("pi")->evaluate(), 3.1415926535);

    ASSERT_NO_THROW(pt::register_constant("e", 2.7));
    ASSERT_TRUE(pt::is_constant("e"));
    ASSERT_EQ(pt::get_constant("e")->evaluate(), 2.7);
}


TEST(RegisterAndFind, Unary)
{
    ASSERT_NO_THROW(pt::register_unary("sin", (double(*)(double))std::sin));
    ASSERT_TRUE(pt::is_unary_operator("sin"));
    ASSERT_NO_THROW(pt::get_unary_operator("sin"));

    ASSERT_NO_THROW(pt::register_unary("-", std::negate<double>()));
    ASSERT_TRUE(pt::is_unary_operator("-"));
    ASSERT_NO_THROW(pt::get_unary_operator("-"));
}

TEST(RegisterAndFind, Binary)
{
    ASSERT_NO_THROW(pt::register_binary("^", (double(*)(double, double))std::pow, 0));
    ASSERT_TRUE(pt::is_binary_operator("^"));
    ASSERT_NO_THROW(pt::get_binary_operator("^"));

    ASSERT_NO_THROW(pt::register_binary("+", std::plus<double>(), 5));
    ASSERT_TRUE(pt::is_binary_operator("+"));
    ASSERT_NO_THROW(pt::get_binary_operator("+"));
}
