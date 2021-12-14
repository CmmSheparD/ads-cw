#include "parsing-table.hh"

#include <cctype>
#include <stdexcept>

#include "list.hh"

namespace infix_parsing {

data_structs::List<ParsingTable::ConstantEntry> ParsingTable::constants_;
data_structs::List<ParsingTable::UnaryOperatorEntry> ParsingTable::unary_operators_;
data_structs::List<ParsingTable::BinaryOperatorEntry> ParsingTable::binary_operators_;


bool ParsingTable::is_valid_name(const std::string &name)
{
    if (name.length() == 0)
        return false;
    if (std::isdigit(name[0]))
        return false;
    for (auto c : name) {
        if (!std::isgraph(c))
            return false;
    }
    return true;
}


void ParsingTable::register_constant(const std::string &name, const double value)
{
    if (!is_valid_name(name))
        throw InvalidNameError(name);
    constants_.push_back({name, value});
}

void ParsingTable::register_unary(const std::string &name, std::function<double (double)> f)
{
    if (!is_valid_name(name))
        throw InvalidNameError(name);
    if (!f)
        throw std::invalid_argument("Operator cannot be null.");
    unary_operators_.push_back({name, f});
}

void ParsingTable::register_binary(const std::string &name, std::function<double (double, double)> f)
{
    if (!is_valid_name(name))
        throw InvalidNameError(name);
    if (!f)
        throw std::invalid_argument("Operator cannot be null.");
    binary_operators_.push_back({name, f});
}


bool ParsingTable::is_constant(const std::string &name)
{
    if (!is_valid_name(name))
        return false;
    for (auto &c : constants_) {
        if (c.name == name)
            return true;
    }
    return false;
}

bool ParsingTable::is_unary_operator(const std::string &name)
{
    if (!is_valid_name(name))
        return false;
    for (auto &c : unary_operators_) {
        if (c.name == name)
            return true;
    }
    return false;
}

bool ParsingTable::is_binary_operator(const std::string &name)
{
    if (!is_valid_name(name))
        return false;
    for (auto &c : binary_operators_) {
        if (c.name == name)
            return true;
    }
    return false;
}


double ParsingTable::get_constant(const std::string &name)
{
    if (!is_valid_name(name))
        throw InvalidNameError(name);
    for (auto &c : constants_) {
        if (name == c.name)
            return c.value;
    }
    throw NameSearchError(name);
}

std::function<double(const double)> ParsingTable::get_unary_operator(const std::string &name)
{
    if (!is_valid_name(name))
        throw InvalidNameError(name);
    for (auto &op : unary_operators_) {
        if (name == op.name)
            return op.func;
    }
    throw NameSearchError(name);
}

std::function<double(const double, const double)> ParsingTable::get_binary_operator(const std::string &name)
{
    if (!is_valid_name(name))
        throw InvalidNameError(name);
    for (auto &op : binary_operators_) {
        if (name == op.name)
            return op.func;
    }
    throw NameSearchError(name);
}

}   // namespace infix_parsing
