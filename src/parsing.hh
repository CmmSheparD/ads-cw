#pragma once
#ifndef PARSING_HH
#define PARSING_HH

#include <string>

#include "calculation-tree.hh"

namespace infix_parsing {

void init_table();


/*
 * Returns an expression, which evaluation will calculate the expression
 * stored in the string.
 */
std::shared_ptr<calculation::Operand> parse_expression(const std::string &string, size_t start = 0);

}   // namespace infix_parsing

#endif  // PARSING_HH
