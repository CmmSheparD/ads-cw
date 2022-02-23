#include "parsing.hh"

#include <cmath>
#include <functional>
#include <memory>
#include <string>

#include "list.hh"

#include "calculation-tree.hh"
#include "parsing-exceptions.hh"
#include "parsing-table.hh"

namespace infix_parsing {

using table = ParsingTable;

using data_structs::List;

using calculation::Operand;
using calculation::Constant;
using calculation::Expression;

using calculation::Operator;
using calculation::UnaryOperator;
using calculation::BinaryOperator;

double adapter_ctg(double arg)
{
    return 1.0 / std::tan(arg);
}

void init_table()
{
    table::register_constant("pi", 3.141592653589793);
    table::register_constant("e",  2.718281828459045);

    table::register_unary("-", std::negate<double>());
    table::register_unary("abs", (double (*)(double))std::abs);
    table::register_unary("sin", (double (*)(double))std::sin);
    table::register_unary("cos", (double (*)(double))std::cos);
    table::register_unary("tg", (double (*)(double))std::tan);
    table::register_unary("ctg", adapter_ctg);
    table::register_unary("ln", (double (*)(double))std::log);
    table::register_unary("log", (double (*)(double))std::log10);
    table::register_unary("sqrt", (double (*)(double))std::sqrt);

    table::register_binary("^", (double (*)(double, double))std::pow, 0);
    table::register_binary("*", std::multiplies<double>(), 1);
    table::register_binary("/", std::divides<double>(), 1);
    table::register_binary("+", std::plus<double>(), 2);
    table::register_binary("-", std::minus<double>(), 2);
}


size_t skip_spaces(const std::string &string, size_t start)
{
    const size_t len = string.size();
    size_t pos = start;
    while (pos < len && std::isspace(string[pos]))
        ++pos;
    return pos;
}

std::shared_ptr<Constant> parse_value(const std::string &string, size_t &start)
{
    size_t processed = 0;
    double val;
    try {
        std::string buffer(string, start, string.size() - start);
        val = std::stod(buffer, &processed);
    } catch (std::out_of_range &) {
        throw TooBigNumber(start);
    }
    std::shared_ptr<Constant> res = std::make_shared<Constant>(
        val,
        std::string(string, start, processed)
    );
    start += processed;
    return res;
}

std::shared_ptr<Constant> parse_constant(const std::string &string, size_t &start)
{
    const size_t len = string.length();
    size_t pos = start;
    std::string copy;
    copy += string[pos];
    while (!table::is_constant(copy)) {
        if (++pos == len)
            throw UnexpectedEndOfExpression(pos);
        copy += string[pos];
    }
    start = pos + 1;
    return table::get_constant(copy);
}

std::shared_ptr<UnaryOperator> parse_unary(const std::string &string, size_t &start)
{
    const size_t len = string.length();
    size_t pos = start;
    std::string copy;
    copy += string[pos];
    while (!table::is_unary_operator(copy)) {
        if (++pos == len)
            throw UnexpectedEndOfExpression(pos);
        copy += string[pos];
    }
    start = pos + 1;
    return table::get_unary_operator(copy);
}

std::shared_ptr<Operand> parse_operand(const std::string &string, size_t &start)
{
    const size_t len = string.length();
    if (start >= len)
        throw UnexpectedEndOfExpression(start);
    size_t pos = skip_spaces(string, start);
    if (pos == len)
        throw OperandExpectationUnsatisfied(pos);

    size_t backup_pos = pos;
    std::shared_ptr<Operand> res;
    if (string[pos] == '(') {
        size_t counter = 1;
        for (++pos; pos < len && counter > 0; ++pos) {
            if (string[pos] == '(')
                ++counter;
            else if (string[pos] == ')')
                --counter;
        }
        if (pos == len && counter != 0)
            throw UnexpectedEndOfExpression(pos);
        std::string inner(string, backup_pos + 1, pos - backup_pos - 2);
        try {
            res = parse_expression(inner);
        } catch (ParserError &e) {
            // Restoring absolute position in the string and re-throwing
            e.position += backup_pos + 1;
            throw e;
        }
    } else if (table::is_starting_digit(string[pos])) {
        res = parse_value(string, pos);
    } else {
        try {
            std::shared_ptr<UnaryOperator> tmp = parse_unary(string, pos);
            tmp->set_operand(parse_operand(string, pos));

            std::shared_ptr<Expression> exp(new Expression);
            exp->set_root(tmp);
            res = exp;
        } catch (const ParserError&) {
            pos = backup_pos;
            try {
                res = parse_constant(string, pos);
            } catch (const UnexpectedEndOfExpression&) {
                throw OperandExpectationUnsatisfied(backup_pos);
            }
        }
    }
    start = pos;
    return res;
}


std::shared_ptr<BinaryOperator> parse_operator(const std::string &string, size_t &start)
{
    const size_t len = string.length();
    if (start >= len)
        throw UnexpectedEndOfExpression(len);
    size_t pos = skip_spaces(string, start);
    if (pos == len)
        throw BinaryExpectationUnsatisfied(start);

    std::string copy;
    copy += string[pos];
    while (!table::is_binary_operator(copy)) {
        if (++pos == len)
            throw UnexpectedEndOfExpression(pos);
        copy += string[pos];
    }
    start = pos + 1;
    return table::get_binary_operator(copy);
}

std::shared_ptr<Operand> create_tree_from_parser_lists(List<std::shared_ptr<Operand>> operands, List<std::shared_ptr<BinaryOperator>> operators)
{
    if (operands.size() == 1)
        return *operands.begin();
    std::shared_ptr<Expression> res = std::make_shared<Expression>();
    List<std::shared_ptr<Operand>>::Iterator operand = operands.begin();
    List<std::shared_ptr<Operand>>::Iterator left_operand = operand++;
    List<std::shared_ptr<BinaryOperator>>::Iterator op_it = operators.begin();
    List<std::shared_ptr<BinaryOperator>>::Iterator hang_point = op_it++;
    while (op_it != operators.end()) {
        if ((*hang_point)->order() <= (*op_it)->order()) {
            hang_point = op_it;
            left_operand = operand;
        }
        ++op_it;
        ++operand;
    }
    res->set_root(*hang_point);
    (*hang_point)->set_left(create_tree_from_parser_lists(
        {operands.begin(), ++left_operand},
        {operators.begin(), hang_point}));
    (*hang_point)->set_right(create_tree_from_parser_lists(
        {left_operand, operands.end()},
        {++hang_point, operators.end()}));
    return res;
}

std::shared_ptr<Operand> parse_expression(const std::string &string, size_t start)
{
    const size_t len = string.length();
    if (len == 0)
        return std::shared_ptr<Operand>(new Constant(0));
    /*
     * At first, there will definetly be either a constant or an unary operator
     * with its own operand. If not, there's no parsable expression anyway,
     * so exception fallback from parse_operand is acceptable.
     */
    size_t pos = start;
    std::shared_ptr<Operand> tmpoperand;
    std::shared_ptr<BinaryOperator> tmpoperator;
    List<std::shared_ptr<BinaryOperator>> operators;
    List<std::shared_ptr<Operand>> operands;
    do {
        pos = skip_spaces(string, pos);
        tmpoperand = parse_operand(string, pos);
        operands.push_back(tmpoperand);
        pos = skip_spaces(string, pos);
        if (pos < len) {
            tmpoperator = parse_operator(string, pos);
            operators.push_back(tmpoperator);
        } else {
            break;
        }
    } while (true);

    std::shared_ptr<Operand> res;
    if (operands.size() == 1)
        res = *operands.begin();
    else
        res = create_tree_from_parser_lists(operands, operators);
    return res;
}

}   // namespace infix_parsing
